# 🛡️ MiniQT - Análise de Segurança e Guia de Features Avançadas

## 📊 ANÁLISE DE SEGURANÇA

### ✅ **Segurança de Memória - STATUS: BOM**

#### **Análise Detalhada:**

**1. Gerenciamento de Memória - Correto ✅**
```c
// backend.c - CORRETO
t_backend_context *mqt_backend_create(...) {
    ctx = malloc(sizeof(t_backend_context));
    if (!ctx)
        return (NULL);  // ✅ Verifica malloc
    
    ctx->title = strdup(title ? title : "MiniQT Window");
    // ✅ Proteção contra NULL
    
    if (!ctx->ops) {
        free(ctx->title);  // ✅ Cleanup adequado
        free(ctx);
        return (NULL);
    }
    // ...
}

void mqt_backend_destroy(t_backend_context *ctx) {
    if (!ctx)
        return;  // ✅ Proteção NULL
    
    if (ctx->ops && ctx->ops->destroy)
        ctx->ops->destroy(ctx);  // ✅ Chama destrutor do backend
    
    if (ctx->title)
        free(ctx->title);  // ✅ Verifica antes de liberar
    free(ctx);
}
```

**2. Framebuffer - Correto com Rollback ✅**
```c
// mqt_framebuffer.c - CORRETO
t_framebuffer *mqt_fb_create(int width, int height) {
    fb = malloc(sizeof(t_framebuffer));
    if (!fb)
        return (NULL);
    
    fb->pixels = malloc(sizeof(t_pixel *) * height);
    if (!fb->pixels) {
        free(fb);  // ✅ Rollback
        return (NULL);
    }
    
    y = 0;
    while (y < height) {
        fb->pixels[y] = malloc(sizeof(t_pixel) * width);
        if (!fb->pixels[y]) {
            while (--y >= 0)
                free(fb->pixels[y]);  // ✅ Limpa alocações anteriores
            free(fb->pixels);
            free(fb);
            return (NULL);
        }
        y++;
    }
    return (fb);
}
```

#### **Problemas Encontrados e Soluções:**

**⚠️ PROBLEMA 1: Possível leak em backend_sdl2.c**
```c
// ATUAL - Pode vazar memória se init falhar
static int sdl2_init(t_backend_context *ctx, ...) {
    data = malloc(sizeof(t_sdl2_data));
    if (!data) {
        SDL_Quit();
        return (-1);
    }
    
    data->window = SDL_CreateWindow(...);
    if (!data->window) {
        free(data);
        SDL_Quit();
        return (-1);  // ✅ OK
    }
    
    data->renderer = SDL_CreateRenderer(...);
    if (!data->renderer) {
        SDL_DestroyWindow(data->window);  // ✅ OK
        free(data);
        SDL_Quit();
        return (-1);
    }
    
    ctx->backend_data = data;
    return (0);
}
```
**Status:** ✅ Atualmente CORRETO - Todos os caminhos fazem cleanup adequado

**⚠️ PROBLEMA 2: strdup pode falhar**
```c
// backend.c linha 43
ctx->title = strdup(title ? title : "MiniQT Window");
// ❌ Não verifica se strdup retornou NULL
```

**CORREÇÃO NECESSÁRIA:**
```c
ctx->title = strdup(title ? title : "MiniQT Window");
if (!ctx->title) {
    free(ctx);
    return (NULL);
}
```

---

### 🧵 **Segurança de Threads - STATUS: NÃO THREAD-SAFE**

#### **Análise:**

**❌ PROBLEMA: API não é thread-safe**

1. **Sem proteção de mutex**
2. **Backend SDL2 não é thread-safe**
3. **Framebuffer compartilhado sem locks**
4. **Eventos podem ter race conditions**

#### **Cenários Perigosos:**

```c
// ❌ PERIGOSO - Duas threads desenhando
void *thread1(void *arg) {
    t_backend_context *ctx = (t_backend_context *)arg;
    mqt_draw_rectangle(ctx, ...);  // Race condition!
}

void *thread2(void *arg) {
    t_backend_context *ctx = (t_backend_context *)arg;
    mqt_draw_text(ctx, ...);  // Race condition!
}
```

**O que pode acontecer:**
- Corrupção do framebuffer
- Crashes no SDL2
- Desenhos sobrepostos/incompletos
- Memory corruption

#### **Soluções Propostas:**

**OPÇÃO 1: Documentar limitação (Simples)**
```c
/*
 * NOTA: Esta API NÃO é thread-safe.
 * Todas as operações de desenho devem ocorrer na mesma thread.
 * SDL2 requer que operações gráficas sejam na thread principal.
 */
```

**OPÇÃO 2: Adicionar proteção com mutex (Médio)**
```c
typedef struct s_backend_context {
    // ... campos existentes ...
    pthread_mutex_t render_mutex;
} t_backend_context;

void mqt_begin_frame(t_backend_context *ctx) {
    pthread_mutex_lock(&ctx->render_mutex);
    if (ctx && ctx->ops && ctx->ops->begin_frame)
        ctx->ops->begin_frame(ctx);
}

void mqt_end_frame(t_backend_context *ctx) {
    if (ctx && ctx->ops && ctx->ops->end_frame)
        ctx->ops->end_frame(ctx);
    pthread_mutex_unlock(&ctx->render_mutex);
}
```

**OPÇÃO 3: Command Queue (Avançado)**
```c
// Thread worker enfileira comandos
mqt_queue_draw_rect(ctx, rect, color);
mqt_queue_draw_text(ctx, x, y, text, color);

// Thread principal processa fila
void render_thread() {
    while (running) {
        mqt_process_command_queue(ctx);
        mqt_present_frame(ctx);
    }
}
```

**RECOMENDAÇÃO:** Usar OPÇÃO 1 (documentar) + garantir uso single-threaded.
SDL2 já tem limitações de thread, então é aceitável.

---

## 🎨 IMPLEMENTAÇÃO DE FEATURES AVANÇADAS

### 1. **Sistema de Janelas (Window Manager)**

```c
// lib/miniqt_widgets/window.h

typedef struct s_window {
    char        *title;
    t_rect      bounds;
    t_color     bg_color;
    t_color     title_bar_color;
    t_color     border_color;
    int         is_focused;
    int         is_visible;
    int         is_draggable;
    int         is_resizable;
    void        *user_data;
    
    // Callbacks
    void (*on_close)(struct s_window *win);
    void (*on_resize)(struct s_window *win, int w, int h);
    void (*on_focus)(struct s_window *win, int focused);
} t_window;

// Criação
t_window *mqt_window_create(const char *title, int x, int y, 
                             int width, int height);

// Renderização
void mqt_window_draw(t_backend_context *ctx, t_window *win);

// Gerenciamento
void mqt_window_set_position(t_window *win, int x, int y);
void mqt_window_set_size(t_window *win, int width, int height);
void mqt_window_set_title(t_window *win, const char *title);
void mqt_window_show(t_window *win);
void mqt_window_hide(t_window *win);
void mqt_window_focus(t_window *win);

// Hit testing
int mqt_window_contains_point(t_window *win, int x, int y);
int mqt_window_title_bar_contains(t_window *win, int x, int y);

// Destruição
void mqt_window_destroy(t_window *win);
```

**Implementação do Draw:**
```c
void mqt_window_draw(t_backend_context *ctx, t_window *win) {
    if (!win || !win->is_visible)
        return;
    
    // Fundo da janela
    mqt_draw_rectangle_filled(ctx, win->bounds, win->bg_color);
    
    // Barra de título
    t_rect title_bar = mqt_make_rect(
        win->bounds.x,
        win->bounds.y,
        win->bounds.width,
        25  // Altura da barra
    );
    mqt_draw_rectangle_filled(ctx, title_bar, win->title_bar_color);
    
    // Título
    if (win->title) {
        mqt_draw_string(ctx, 
            win->bounds.x + 5,
            win->bounds.y + 8,
            win->title,
            mqt_color(255, 255, 255, 255));
    }
    
    // Botão fechar (X)
    int close_x = win->bounds.x + win->bounds.width - 20;
    int close_y = win->bounds.y + 5;
    mqt_draw_rectangle_filled(ctx,
        mqt_make_rect(close_x, close_y, 15, 15),
        mqt_color(220, 50, 50, 255));
    mqt_draw_string(ctx, close_x + 3, close_y + 3, "X",
        mqt_color(255, 255, 255, 255));
    
    // Borda
    if (win->is_focused) {
        mqt_draw_rectangle(ctx, win->bounds,
            mqt_color(100, 150, 255, 255));  // Azul se focado
    } else {
        mqt_draw_rectangle(ctx, win->bounds, win->border_color);
    }
}
```

---

### 2. **Sistema de Input Fields (Text Input)**

```c
// lib/miniqt_widgets/input_field.h

typedef struct s_input_field {
    t_rect      bounds;
    char        *text;
    size_t      max_length;
    size_t      cursor_pos;
    int         is_focused;
    int         is_password;  // Mostra ***
    t_color     bg_color;
    t_color     text_color;
    t_color     border_color;
    t_color     cursor_color;
    
    // Callbacks
    void (*on_change)(struct s_input_field *field, const char *text);
    void (*on_submit)(struct s_input_field *field, const char *text);
} t_input_field;

// API
t_input_field *mqt_input_create(int x, int y, int width, size_t max_len);
void mqt_input_draw(t_backend_context *ctx, t_input_field *field);
void mqt_input_handle_key(t_input_field *field, int key_code);
void mqt_input_set_text(t_input_field *field, const char *text);
const char *mqt_input_get_text(t_input_field *field);
void mqt_input_clear(t_input_field *field);
void mqt_input_destroy(t_input_field *field);
```

**Implementação:**
```c
t_input_field *mqt_input_create(int x, int y, int width, size_t max_len) {
    t_input_field *field = malloc(sizeof(t_input_field));
    if (!field)
        return (NULL);
    
    field->bounds = mqt_make_rect(x, y, width, 30);
    field->text = calloc(max_len + 1, sizeof(char));
    if (!field->text) {
        free(field);
        return (NULL);
    }
    
    field->max_length = max_len;
    field->cursor_pos = 0;
    field->is_focused = 0;
    field->is_password = 0;
    field->bg_color = mqt_color(255, 255, 255, 255);
    field->text_color = mqt_color(0, 0, 0, 255);
    field->border_color = mqt_color(150, 150, 150, 255);
    field->cursor_color = mqt_color(0, 0, 0, 255);
    field->on_change = NULL;
    field->on_submit = NULL;
    
    return (field);
}

void mqt_input_draw(t_backend_context *ctx, t_input_field *field) {
    // Fundo
    mqt_draw_rectangle_filled(ctx, field->bounds, field->bg_color);
    
    // Borda (destaque se focado)
    t_color border = field->is_focused ?
        mqt_color(100, 150, 255, 255) : field->border_color;
    mqt_draw_rectangle(ctx, field->bounds, border);
    
    // Texto
    const char *display_text = field->is_password ?
        "********" : field->text;
    
    mqt_draw_string(ctx,
        field->bounds.x + 5,
        field->bounds.y + 10,
        display_text,
        field->text_color);
    
    // Cursor piscante (se focado)
    if (field->is_focused) {
        int cursor_x = field->bounds.x + 5 + (field->cursor_pos * 8);
        mqt_draw_rectangle_filled(ctx,
            mqt_make_rect(cursor_x, field->bounds.y + 8, 2, 14),
            field->cursor_color);
    }
}

void mqt_input_handle_key(t_input_field *field, int key_code) {
    if (!field->is_focused)
        return;
    
    size_t len = strlen(field->text);
    
    // Backspace
    if (key_code == 8 && field->cursor_pos > 0) {
        memmove(&field->text[field->cursor_pos - 1],
                &field->text[field->cursor_pos],
                len - field->cursor_pos + 1);
        field->cursor_pos--;
        if (field->on_change)
            field->on_change(field, field->text);
    }
    // Delete
    else if (key_code == 127 && field->cursor_pos < len) {
        memmove(&field->text[field->cursor_pos],
                &field->text[field->cursor_pos + 1],
                len - field->cursor_pos);
        if (field->on_change)
            field->on_change(field, field->text);
    }
    // Enter
    else if (key_code == 13) {
        if (field->on_submit)
            field->on_submit(field, field->text);
    }
    // Seta esquerda
    else if (key_code == 260 && field->cursor_pos > 0) {
        field->cursor_pos--;
    }
    // Seta direita
    else if (key_code == 261 && field->cursor_pos < len) {
        field->cursor_pos++;
    }
    // Caractere normal
    else if (key_code >= 32 && key_code <= 126 && len < field->max_length) {
        memmove(&field->text[field->cursor_pos + 1],
                &field->text[field->cursor_pos],
                len - field->cursor_pos + 1);
        field->text[field->cursor_pos] = (char)key_code;
        field->cursor_pos++;
        if (field->on_change)
            field->on_change(field, field->text);
    }
}
```

---

### 3. **Sistema de Botões Interativos**

```c
// lib/miniqt_widgets/button.h

typedef enum e_button_state {
    BUTTON_NORMAL,
    BUTTON_HOVER,
    BUTTON_PRESSED,
    BUTTON_DISABLED
} t_button_state;

typedef struct s_button {
    char            *label;
    t_rect          bounds;
    t_button_state  state;
    int             is_enabled;
    
    t_color         normal_color;
    t_color         hover_color;
    t_color         pressed_color;
    t_color         disabled_color;
    t_color         text_color;
    
    void (*on_click)(struct s_button *btn);
} t_button;

// API
t_button *mqt_button_create(const char *label, int x, int y, 
                             int width, int height);
void mqt_button_draw(t_backend_context *ctx, t_button *btn);
void mqt_button_update(t_button *btn, int mouse_x, int mouse_y, 
                        int mouse_down);
int mqt_button_is_hovered(t_button *btn, int mouse_x, int mouse_y);
void mqt_button_set_enabled(t_button *btn, int enabled);
void mqt_button_destroy(t_button *btn);
```

**Implementação:**
```c
void mqt_button_draw(t_backend_context *ctx, t_button *btn) {
    t_color bg_color;
    
    // Escolhe cor baseado no estado
    switch (btn->state) {
        case BUTTON_HOVER:
            bg_color = btn->hover_color;
            break;
        case BUTTON_PRESSED:
            bg_color = btn->pressed_color;
            break;
        case BUTTON_DISABLED:
            bg_color = btn->disabled_color;
            break;
        default:
            bg_color = btn->normal_color;
    }
    
    // Desenha fundo
    mqt_draw_rectangle_filled(ctx, btn->bounds, bg_color);
    
    // Desenha borda
    mqt_draw_rectangle(ctx, btn->bounds,
        mqt_color(100, 100, 100, 255));
    
    // Desenha texto centralizado
    if (btn->label) {
        int text_width = strlen(btn->label) * 8;  // Aprox
        int text_x = btn->bounds.x + (btn->bounds.width - text_width) / 2;
        int text_y = btn->bounds.y + (btn->bounds.height - 14) / 2;
        
        mqt_draw_string(ctx, text_x, text_y, btn->label, btn->text_color);
    }
}

void mqt_button_update(t_button *btn, int mouse_x, int mouse_y, 
                        int mouse_down) {
    if (!btn->is_enabled) {
        btn->state = BUTTON_DISABLED;
        return;
    }
    
    int hovered = mqt_button_is_hovered(btn, mouse_x, mouse_y);
    
    if (hovered && mouse_down) {
        btn->state = BUTTON_PRESSED;
    } else if (hovered) {
        btn->state = BUTTON_HOVER;
    } else {
        btn->state = BUTTON_NORMAL;
    }
}

int mqt_button_is_hovered(t_button *btn, int mouse_x, int mouse_y) {
    return (mouse_x >= btn->bounds.x &&
            mouse_x <= btn->bounds.x + btn->bounds.width &&
            mouse_y >= btn->bounds.y &&
            mouse_y <= btn->bounds.y + btn->bounds.height);
}
```

---

### 4. **Sistema de Layouts**

```c
// lib/miniqt_widgets/layout.h

typedef enum e_layout_type {
    LAYOUT_VERTICAL,
    LAYOUT_HORIZONTAL,
    LAYOUT_GRID
} t_layout_type;

typedef struct s_layout {
    t_layout_type   type;
    t_rect          bounds;
    void            **widgets;  // Array de ponteiros para widgets
    int             widget_count;
    int             widget_capacity;
    int             spacing;
    int             padding;
} t_layout;

// API
t_layout *mqt_layout_create(t_layout_type type, t_rect bounds);
void mqt_layout_add_widget(t_layout *layout, void *widget);
void mqt_layout_arrange(t_layout *layout);  // Recalcula posições
void mqt_layout_draw(t_backend_context *ctx, t_layout *layout);
void mqt_layout_destroy(t_layout *layout);
```

**Exemplo de uso:**
```c
// Criar layout vertical
t_layout *vbox = mqt_layout_create(LAYOUT_VERTICAL,
    mqt_make_rect(10, 10, 300, 400));
vbox->spacing = 10;
vbox->padding = 10;

// Adicionar widgets
t_input_field *username = mqt_input_create(0, 0, 280, 50);
t_input_field *password = mqt_input_create(0, 0, 280, 50);
t_button *login_btn = mqt_button_create("Login", 0, 0, 280, 40);

mqt_layout_add_widget(vbox, username);
mqt_layout_add_widget(vbox, password);
mqt_layout_add_widget(vbox, login_btn);

// Layout automaticamente posiciona tudo
mqt_layout_arrange(vbox);

// No loop de renderização
mqt_layout_draw(ctx, vbox);
```

---

### 5. **Sistema de Eventos Melhorado**

```c
// lib/miniqt_backend/events.h

typedef struct s_event_handler {
    void (*on_mouse_move)(int x, int y);
    void (*on_mouse_down)(int x, int y, int button);
    void (*on_mouse_up)(int x, int y, int button);
    void (*on_key_down)(int key_code);
    void (*on_key_up)(int key_code);
    void (*on_text_input)(const char *text);
    void (*on_window_resize)(int width, int height);
} t_event_handler;

// Dispatcher de eventos
void mqt_dispatch_events(t_backend_context *ctx, t_event_handler *handler);
```

---

## 📋 PLANO DE IMPLEMENTAÇÃO

### **Fase 1: Correções de Segurança** (PRIORITÁRIO)
1. ✅ Adicionar verificação de `strdup` em `backend.c`
2. ✅ Adicionar documentação thread-safety
3. ✅ Adicionar testes de vazamento de memória

### **Fase 2: Widgets Básicos**
1. Sistema de janelas (`window.c/h`)
2. Botões interativos (`button.c/h`)
3. Input fields (`input_field.c/h`)

### **Fase 3: Sistema de Layouts**
1. Layout vertical/horizontal
2. Layout em grade
3. Auto-dimensionamento

### **Fase 4: Sistema de Eventos**
1. Event dispatcher
2. Callbacks por widget
3. Event bubbling (propagação)

### **Fase 5: Widgets Avançados**
1. Checkboxes
2. Radio buttons
3. Sliders
4. Dropdowns
5. List views
6. Scroll bars

---

## 🔧 CORREÇÕES IMEDIATAS NECESSÁRIAS

Quer que eu implemente as correções de segurança agora?
