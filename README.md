# 🎨 MiniQT - Framework Gráfico Multi-Plataforma em C

Um framework gráfico leve e portátil escrito em C puro, com suporte a múltiplos backends de renderização (Terminal ANSI e SDL2).

---

## 🎯 Quick Start

### 📡 Servidor SSH (42 School)
```bash
cd examples && make                    # Terminal backend (funciona via SSH!)
./basic_demo/basic_demo
```

### 💻 Máquina Local (com GUI)
```bash
cd examples && make SDL2=1             # SDL2 backend (janelas gráficas)
./basic_demo/basic_demo
```

**→ [QUICKSTART.md](QUICKSTART.md)** - Guia completo de uso

---

## ✨ Características

- 🖥️ **Multi-backend:** Terminal (ANSI) e SDL2
- 🔧 **Zero dependências externas:** SDL2 compilado localmente
- 🎯 **Cross-platform:** Linux, macOS, Windows, FreeBSD
- 📦 **Totalmente standalone:** Binário único sem libs externas necessárias
- 🔄 **Detecção automática:** Escolhe melhor backend baseado no ambiente
- 🎨 **API unificada:** Mesmo código funciona em todos backends
- ⚡ **Modular:** Fácil adicionar novos backends

## 🚀 Quick Start

### 1. Clonar/Setup Básico

```bash
cd miniQT
```

### 2. Compilar (somente Terminal backend)

```bash
make
./miniqt_example
```

### 3. Compilar com SDL2 (janelas reais)

```bash
# Setup SDL2 (primeira vez apenas)
chmod +x setup_sdl2.sh
./setup_sdl2.sh

# Compilar com SDL2
make SDL2=1
./miniqt_example
```

## 📁 Estrutura do Projeto

```
miniQT/
├── include/
│   └── miniqt.h                    # Header principal
├── lib/
│   ├── libft/                      # Biblioteca libft
│   ├── ft_printf/                  # Printf customizado
│   ├── miniqt_render/              # Primitivas de desenho
│   │   ├── mqt_terminal.h/c       # Controle de terminal ANSI
│   │   ├── mqt_framebuffer.h/c    # Framebuffer virtual
│   │   └── mqt_draw.h/c           # Funções de desenho
│   ├── miniqt_backend/             # Sistema de backends
│   │   ├── platform.h/c           # Detecção de OS
│   │   ├── backend.h/c            # Interface abstrata
│   │   ├── backend_terminal.c     # Backend Terminal
│   │   ├── backend_sdl2.c         # Backend SDL2
│   │   └── backend_factory.h/c    # Factory pattern
│   └── SDL2/                       # SDL2 local (após setup)
├── src/
│   └── example.c                   # Programa de exemplo
├── setup_sdl2.sh                   # Script de setup SDL2
├── ARCHITECTURE.md                 # Documentação da arquitetura
├── Makefile
└── README.md
```

## 🎯 Uso Básico

### Exemplo Mínimo

```c
#include "lib/miniqt_backend/backend_factory.h"

int main(void)
{
    t_backend_context *ctx;
    t_event event;
    
    // Criar backend automaticamente
    ctx = mqt_create_auto_backend(800, 600, "Minha Aplicação");
    if (!ctx)
        return 1;
    
    // Loop principal
    int running = 1;
    while (running)
    {
        // Processar eventos
        while (mqt_poll_event_ctx(ctx, &event))
        {
            if (event.type == MQT_EVENT_QUIT)
                running = 0;
        }
        
        // Desenhar
        mqt_begin_frame(ctx);
        mqt_clear_screen_color(ctx, mqt_color(30, 30, 40, 255));
        
        mqt_draw_rectangle_filled(ctx,
            mqt_make_rect(100, 100, 200, 150),
            mqt_color(100, 150, 250, 255));
            
        mqt_draw_string(ctx, 150, 250, "Hello MiniQT!", 
            mqt_color(255, 255, 255, 255));
        
        mqt_end_frame(ctx);
    }
    
    mqt_backend_destroy(ctx);
    return 0;
}
```

### Escolher Backend Específico

```c
// Forçar uso do Terminal
ctx = mqt_create_backend(MQT_BACKEND_TERMINAL, 100, 30, "Terminal UI");

// Forçar uso do SDL2
ctx = mqt_create_backend(MQT_BACKEND_SDL2, 800, 600, "SDL2 Window");
```

### Listar Backends Disponíveis

```c
mqt_list_available_backends();
```

## 🔧 Compilação

### Makefile Targets

```bash
make          # Compila sem SDL2 (apenas Terminal)
make SDL2=1   # Compila com SDL2
make clean    # Remove objetos
make fclean   # Remove tudo
make re       # Recompila do zero
```

### Flags de Compilação

O sistema detecta automaticamente:
- Se SDL2 está instalado em `lib/SDL2/`
- Sistema operacional (Linux, macOS, Windows)
- Ambiente gráfico disponível (X11, Wayland)

## 🎨 API de Desenho

### Cores

```c
t_color cor = mqt_color(r, g, b, a);  // RGBA 0-255
```

### Primitivas

```c
// Retângulos
mqt_draw_rectangle(ctx, rect, cor);              // Borda
mqt_draw_rectangle_filled(ctx, rect, cor);       // Preenchido

// Texto
mqt_draw_string(ctx, x, y, "texto", cor);

// Limpar tela
mqt_clear_screen_color(ctx, cor);
```

### Eventos

```c
t_event evento;
while (mqt_poll_event_ctx(ctx, &evento))
{
    switch (evento.type)
    {
        case MQT_EVENT_QUIT:
            // Fechar janela
            break;
        case MQT_EVENT_KEY_DOWN:
            // Tecla pressionada: evento.key_code
            break;
        case MQT_EVENT_MOUSE_DOWN:
            // Click mouse: evento.mouse_x, evento.mouse_y
            break;
    }
}
```

## 🏗️ Arquitetura

O projeto usa **padrão Strategy + Factory** para abstrair backends:

```
Sua Aplicação
     ↓
API Abstrata (backend.h)
     ↓
┌────┴────┬──────────┐
↓         ↓          ↓
Terminal  SDL2    (outros)
```

- **Platform Layer:** Detecta OS e ambiente
- **Backend Layer:** Interface comum para todos backends
- **Factory:** Escolhe backend apropriado automaticamente
- **Backends:** Implementações específicas (Terminal, SDL2, etc.)

Veja [ARCHITECTURE.md](ARCHITECTURE.md) para detalhes completos.

## 🔌 Backends Suportados

| Backend    | Status | Plataformas | Requer Display |
|------------|--------|-------------|----------------|
| Terminal   | ✅ Pronto | Todas | Não |
| SDL2       | ✅ Pronto | Linux, macOS, Windows | Sim |
| X11        | 🚧 Futuro | Linux, BSD | Sim |
| Wayland    | 🚧 Futuro | Linux | Sim |
| Win32      | 🚧 Futuro | Windows | Sim |
| Cocoa      | 🚧 Futuro | macOS | Sim |

## 📦 Setup SDL2 Detalhado

### Automático (Recomendado)

```bash
./setup_sdl2.sh
```

### Manual

```bash
cd lib
wget https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-2.28.5.tar.gz
tar -xzf SDL2-2.28.5.tar.gz
cd SDL2-2.28.5
./configure --prefix=$(pwd)/../SDL2 --enable-static --disable-shared
make -j$(nproc)
make install
```

### Verificar Instalação

```bash
ls -la lib/SDL2/lib/libSDL2.a
ls -la lib/SDL2/include/SDL2/
```

## 🐛 Troubleshooting

### SDL2 não compila

```bash
# Instalar dependências mínimas (apenas para compilar)
# Ubuntu/Debian
sudo apt-get install build-essential libx11-dev

# macOS
xcode-select --install

# Então reexecutar
./setup_sdl2.sh
```

### Programa não roda

```bash
# Verificar backend disponível
./miniqt_example  # Mostra qual backend será usado

# Forçar terminal se SDL2 falhar
# Compilar sem SDL2=1
make fclean
make
```

## 📚 Exemplos

Veja [src/example.c](src/example.c) para exemplo completo com:
- Criação automática de backend
- Desenho de formas coloridas
- Processamento de eventos
- Loop de renderização

## 🛠️ Desenvolvimento

### Adicionar Novo Backend

1. Criar `lib/miniqt_backend/backend_NOME.c`
2. Implementar `t_backend_ops` com todas operações
3. Registrar em `backend.c::get_backend_ops()`
4. Atualizar `platform.c` para detecção

Veja [ARCHITECTURE.md](ARCHITECTURE.md) para guia completo.

## 📄 Licença

Ver [LICENSE](LICENSE)

## 👤 Autor

lsarraci - Escola 42

## 🙏 Agradecimentos

- libft e ft_printf da Escola 42
- SDL2 Development Team
- Comunidade C/Graphics Programming
