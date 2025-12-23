# 🔌 MiniQT - Guia de Integração como API

## Como usar MiniQT em outros projetos

### 📦 Método 1: Copiar Diretório Completo

```bash
# No seu projeto
mkdir -p libs
cp -r /path/to/miniQT libs/miniqt

# Estrutura resultante:
# seu_projeto/
# ├── libs/
# │   └── miniqt/
# │       ├── lib/
# │       └── include/
# └── src/
#     └── seu_programa.c
```

### 🔗 Método 2: Compilar como Biblioteca Estática

```bash
# 1. No diretório miniQT
make clean
make libminiqt.a   # (você precisará adicionar este target ao Makefile)

# 2. Copiar arquivos necessários
mkdir -p /seu/projeto/libs/miniqt
cp libminiqt.a /seu/projeto/libs/miniqt/
cp -r include /seu/projeto/libs/miniqt/
cp -r lib /seu/projeto/libs/miniqt/
```

### 📝 Exemplo de Uso em Outro Projeto

**arquivo: meu_programa.c**
```c
#include "libs/miniqt/include/miniqt.h"

int main(void)
{
    t_backend_context *ctx;
    t_event event;
    int running;

    // Criar janela/contexto gráfico
    ctx = mqt_create_auto_backend(800, 600, "Meu Programa");
    if (!ctx)
        return (1);

    running = 1;
    while (running)
    {
        // Processar eventos
        while (mqt_poll_event_ctx(ctx, &event))
        {
            if (event.type == MQT_EVENT_QUIT)
                running = 0;
            else if (event.type == MQT_EVENT_KEY_DOWN)
            {
                if (event.key_code == 27) // ESC
                    running = 0;
            }
        }

        // Renderizar
        mqt_begin_frame(ctx);
        
        // Fundo azul escuro
        mqt_clear_screen_color(ctx, mqt_color(20, 20, 40, 255));
        
        // Retângulo vermelho
        mqt_draw_rectangle_filled(ctx, 
            mqt_make_rect(100, 100, 200, 150),
            mqt_color(220, 50, 50, 255));
        
        // Texto branco
        mqt_draw_string(ctx, 120, 170, 
            "Hello from my program!",
            mqt_color(255, 255, 255, 255));
        
        mqt_end_frame(ctx);
        
        // Controle de frame rate
        usleep(16666); // ~60 FPS
    }

    mqt_backend_destroy(ctx);
    return (0);
}
```

### 🔨 Makefile do Seu Projeto

**Opção A: Compilação simples (sem SDL2)**
```makefile
NAME = meu_programa

SRCS = meu_programa.c
OBJS = $(SRCS:.c=.o)

MINIQT_DIR = libs/miniqt
MINIQT_LIB = $(MINIQT_DIR)/lib

CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -I$(MINIQT_DIR)/include
CFLAGS += -I$(MINIQT_LIB)/miniqt_backend
CFLAGS += -I$(MINIQT_LIB)/miniqt_render
CFLAGS += -I$(MINIQT_LIB)/libft

# Arquivos fonte do MiniQT
MINIQT_SRCS = $(MINIQT_LIB)/miniqt_backend/platform.c \
              $(MINIQT_LIB)/miniqt_backend/backend.c \
              $(MINIQT_LIB)/miniqt_backend/backend_terminal.c \
              $(MINIQT_LIB)/miniqt_backend/backend_sdl2.c \
              $(MINIQT_LIB)/miniqt_backend/backend_factory.c \
              $(MINIQT_LIB)/miniqt_render/mqt_terminal.c \
              $(MINIQT_LIB)/miniqt_render/mqt_framebuffer.c \
              $(MINIQT_LIB)/miniqt_render/mqt_draw.c

MINIQT_OBJS = $(MINIQT_SRCS:.c=.o)

LIBFT = $(MINIQT_LIB)/libft/libft.a
PRINTF = $(MINIQT_LIB)/ft_printf/libftprintf.a

all: $(LIBFT) $(PRINTF) $(NAME)

$(NAME): $(OBJS) $(MINIQT_OBJS)
	$(CC) $(OBJS) $(MINIQT_OBJS) $(LIBFT) $(PRINTF) -o $(NAME)

$(LIBFT):
	make -C $(MINIQT_LIB)/libft

$(PRINTF):
	make -C $(MINIQT_LIB)/ft_printf

clean:
	rm -f $(OBJS) $(MINIQT_OBJS)
	make -C $(MINIQT_LIB)/libft clean
	make -C $(MINIQT_LIB)/ft_printf clean

fclean: clean
	rm -f $(NAME)
	make -C $(MINIQT_LIB)/libft fclean
	make -C $(MINIQT_LIB)/ft_printf fclean

re: fclean all

.PHONY: all clean fclean re
```

**Opção B: Com suporte SDL2**
```makefile
# ... (adicionar ao Makefile acima)

SDL2_DIR = $(MINIQT_LIB)/SDL2
SDL2_INCLUDES = -I$(SDL2_DIR)/include/SDL2
SDL2_LIBS = -L$(SDL2_DIR)/lib -lSDL2

ifdef SDL2
	CFLAGS += -DMQT_USE_SDL2 $(SDL2_INCLUDES)
	LDFLAGS += $(SDL2_LIBS)
endif

$(NAME): $(OBJS) $(MINIQT_OBJS)
	$(CC) $(OBJS) $(MINIQT_OBJS) $(LIBFT) $(PRINTF) $(LDFLAGS) -o $(NAME)
```

### 🎯 API Rápida

#### Inicialização
```c
// Criar backend automaticamente (Terminal ou SDL2)
t_backend_context *ctx = mqt_create_auto_backend(width, height, "título");

// Ou forçar backend específico
ctx = mqt_create_backend(MQT_BACKEND_TERMINAL, 100, 30, "título");
ctx = mqt_create_backend(MQT_BACKEND_SDL2, 800, 600, "título");
```

#### Desenho
```c
// Limpar tela
mqt_clear_screen_color(ctx, mqt_color(r, g, b, a));

// Retângulos
mqt_draw_rectangle(ctx, mqt_make_rect(x, y, w, h), cor);         // Borda
mqt_draw_rectangle_filled(ctx, mqt_make_rect(x, y, w, h), cor);  // Preenchido

// Texto
mqt_draw_string(ctx, x, y, "texto", cor);
```

#### Cores
```c
t_color vermelho = mqt_color(255, 0, 0, 255);
t_color verde    = mqt_color(0, 255, 0, 255);
t_color azul     = mqt_color(0, 0, 255, 255);
t_color branco   = mqt_color(255, 255, 255, 255);
t_color preto    = mqt_color(0, 0, 0, 255);
```

#### Eventos
```c
t_event evento;
while (mqt_poll_event_ctx(ctx, &evento))
{
    switch (evento.type)
    {
        case MQT_EVENT_QUIT:
            // Janela fechada
            break;
        case MQT_EVENT_KEY_DOWN:
            printf("Tecla: %d\n", evento.key_code);
            break;
        case MQT_EVENT_MOUSE_DOWN:
            printf("Click em (%d, %d)\n", evento.mouse_x, evento.mouse_y);
            break;
    }
}
```

#### Frame Control
```c
// Iniciar frame
mqt_begin_frame(ctx);

// ... desenhar tudo ...

// Finalizar e apresentar frame
mqt_end_frame(ctx);
```

#### Limpeza
```c
mqt_backend_destroy(ctx);
```

### 📋 Checklist de Integração

- [ ] Copiar diretório `miniQT` para seu projeto
- [ ] Atualizar includes no seu Makefile
- [ ] Compilar libft e ft_printf
- [ ] (Opcional) Setup SDL2 local
- [ ] Incluir `miniqt.h` no seu código
- [ ] Criar backend com `mqt_create_auto_backend()`
- [ ] Loop de renderização com begin/end frame
- [ ] Processar eventos com `mqt_poll_event_ctx()`
- [ ] Destruir backend ao finalizar

### 🔍 Detecção de Backend em Runtime

```c
t_platform_info info = mqt_detect_platform();
printf("OS: %s\n", info.os_name);
printf("Display: %s\n", info.has_display ? "sim" : "não");
printf("Backend preferido: %s\n", 
       mqt_backend_name(info.preferred_backend));
```

### 💡 Dicas

1. **Sempre verificar retorno de `mqt_create_auto_backend()`**
   - Pode retornar NULL se nenhum backend disponível

2. **Backend Terminal não processa eventos**
   - É ideal para renderização estática ou demos

3. **SDL2 requer display**
   - Em servidores/headless, usa Terminal automaticamente

4. **Performance**
   - Terminal: ~10-30 FPS (limitado por escrita ANSI)
   - SDL2: 60+ FPS (acelerado por GPU)

5. **Portabilidade**
   - Código funciona igual em Linux, macOS, Windows
   - Backend escolhido automaticamente

### 🚀 Exemplos Práticos

Veja o diretório `examples/` no repositório miniQT para:
- Menu interativo
- Jogo simples (Snake/Pong)
- Visualizador de arquivos
- Dashboard de sistema
