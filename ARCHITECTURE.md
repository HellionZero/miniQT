# MiniQT - Sistema de Backends Gráficos Multi-Plataforma

## 📖 Arquitetura do Sistema de Backends

### 1. **Camada de Abstração (Backend Layer)**

O sistema usa o padrão **Strategy** combinado com **Factory** para permitir múltiplos backends gráficos:

```
┌─────────────────────────────────────┐
│     Aplicação (seu código)          │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│   API Abstrata (backend.h)          │
│   - Interface unificada              │
│   - Operações genéricas              │
└──────────────┬──────────────────────┘
               │
    ┌──────────┴──────────┬─────────────┐
    ▼                     ▼             ▼
┌────────┐          ┌─────────┐    ┌────────┐
│Terminal│          │  SDL2   │    │  X11   │
│Backend │          │ Backend │    │Backend │
└────────┘          └─────────┘    └────────┘
```

### 2. **Componentes Principais**

#### **platform.h/c** - Detecção de Plataforma
- Detecta sistema operacional (Linux, macOS, Windows, FreeBSD)
- Verifica ambiente gráfico disponível (X11, Wayland)
- Determina backend preferencial baseado no ambiente

#### **backend.h/c** - Interface Abstrata
- Define estrutura `t_backend_ops` (Virtual Table)
- Declara operações comuns a todos backends:
  - `init()` / `destroy()` - Gerenciamento de ciclo de vida
  - `begin_frame()` / `end_frame()` - Controle de renderização
  - `clear()` - Limpar tela
  - `draw_rect()` / `draw_rect_filled()` - Desenho de retângulos
  - `draw_text()` - Renderização de texto
  - `poll_event()` - Processar eventos (mouse, teclado)

#### **backend_terminal.c** - Implementação Terminal
- Usa ANSI escape sequences
- Framebuffer em memória
- Sempre disponível (fallback)

#### **backend_sdl2.c** - Implementação SDL2
- Usa SDL2 para janelas reais
- Renderização acelerada por hardware
- Eventos de mouse e teclado
- Compilação condicional (`#ifdef MQT_USE_SDL2`)

#### **backend_factory.c/h** - Factory de Backends
- `mqt_create_auto_backend()` - Escolhe backend automaticamente
- `mqt_create_backend()` - Cria backend específico
- `mqt_list_available_backends()` - Lista opções disponíveis

### 3. **Como Funcionam os Contextos Gráficos**

Cada backend mantém seu próprio **contexto** (estado interno):

```c
typedef struct s_backend_context {
    t_graphics_backend  backend_type;  // Tipo do backend
    const t_backend_ops *ops;          // Ponteiro para operações
    void                *backend_data; // Dados específicos do backend
    int                 width, height;
    char                *title;
    int                 is_running;
} t_backend_context;
```

**Backend Terminal:**
```c
typedef struct s_terminal_data {
    t_framebuffer *framebuffer;  // Buffer de caracteres ANSI
} t_terminal_data;
```

**Backend SDL2:**
```c
typedef struct s_sdl2_data {
    SDL_Window   *window;    // Janela nativa do OS
    SDL_Renderer *renderer;  // Contexto de renderização
    SDL_Event    event;      // Sistema de eventos
} t_sdl2_data;
```

### 4. **Fluxo de Execução**

```
1. Aplicação chama: mqt_create_auto_backend()
                         ↓
2. Factory detecta plataforma e ambiente
                         ↓
3. Escolhe backend apropriado (SDL2 ou Terminal)
                         ↓
4. Chama backend->ops->init()
                         ↓
5. Backend inicializa seu contexto específico
                         ↓
6. Retorna t_backend_context* para aplicação
                         ↓
7. Aplicação usa API genérica (mqt_draw_*, etc.)
                         ↓
8. Chamadas são redirecionadas via ops->*
                         ↓
9. Backend específico executa a operação
```

### 5. **Adicionando um Novo Backend**

Para adicionar suporte a Wayland, X11 direto, ou Win32:

**Passo 1:** Criar `backend_wayland.c`
```c
typedef struct s_wayland_data {
    struct wl_display *display;
    struct wl_surface *surface;
    // ... outros recursos Wayland
} t_wayland_data;

static int wayland_init(t_backend_context *ctx, ...) {
    // Conectar ao compositor Wayland
    // Criar surface, etc.
}

// Implementar todas as operações...

const t_backend_ops g_wayland_backend_ops = {
    .init = wayland_init,
    .destroy = wayland_destroy,
    // ...
};
```

**Passo 2:** Registrar em `backend.c`
```c
static const t_backend_ops *get_backend_ops(t_graphics_backend type) {
    // ...
    else if (type == MQT_BACKEND_WAYLAND)
        return (&g_wayland_backend_ops);
    // ...
}
```

**Passo 3:** Atualizar `platform.c`
```c
t_platform_info mqt_detect_platform(void) {
    // ...
    if (info.has_wayland)
        info.preferred_backend = MQT_BACKEND_WAYLAND;
    // ...
}
```

### 6. **Compilação com SDL2**

#### **Sem SDL2 (apenas Terminal):**
```bash
make
```

#### **Com SDL2 local:**
```bash
# 1. Setup SDL2
./setup_sdl2.sh

# 2. Compilar com flag SDL2
make SDL2=1
```

O Makefile detecta automaticamente se SDL2 está em `lib/SDL2/`.

### 7. **Vantagens desta Arquitetura**

✅ **Portabilidade:** Mesmo código funciona em qualquer plataforma
✅ **Modularidade:** Backends são completamente independentes
✅ **Fallback automático:** Se SDL2 falhar, usa Terminal
✅ **Zero dependências externas:** SDL2 compilado localmente
✅ **Extensível:** Fácil adicionar novos backends
✅ **Tipo-seguro:** Interface bem definida em tempo de compilação
✅ **Performance:** Virtual table tem overhead mínimo

### 8. **Uso Prático**

```c
/* Exemplo mínimo */
#include "backend_factory.h"

int main(void) {
    t_backend_context *ctx;
    
    // Backend escolhido automaticamente
    ctx = mqt_create_auto_backend(800, 600, "Minha Janela");
    
    mqt_begin_frame(ctx);
    mqt_clear_screen_color(ctx, mqt_color(30, 30, 40, 255));
    mqt_draw_rectangle_filled(ctx, 
        mqt_make_rect(100, 100, 200, 150),
        mqt_color(100, 150, 250, 255));
    mqt_end_frame(ctx);
    
    // Loop de eventos...
    
    mqt_backend_destroy(ctx);
    return 0;
}
```

### 9. **Estrutura de Diretórios**

```
lib/miniqt_backend/
├── platform.h/c          # Detecção de OS/ambiente
├── backend.h/c           # Interface abstrata
├── backend_terminal.c    # Implementação Terminal
├── backend_sdl2.c        # Implementação SDL2
├── backend_factory.h/c   # Factory pattern
└── (futuros backends)
```

## 🎯 Próximos Passos

1. **Executar setup SDL2:** `./setup_sdl2.sh`
2. **Compilar projeto:** `make SDL2=1`
3. **Testar exemplo:** `./miniqt_example`
4. **Adicionar mais backends conforme necessário**

## 📚 Referências

- [SDL2 Documentation](https://wiki.libsdl.org/)
- [Wayland Protocol](https://wayland.freedesktop.org/)
- Design Pattern: Strategy + Factory
