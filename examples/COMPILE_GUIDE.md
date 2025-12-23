# MiniQT - Guia de Compilação e Backends

## 🎯 Escolhendo o Backend Correto

### Backend Terminal (Recomendado para SSH)
✅ **Use quando**:
- Conectado via SSH sem X11 forwarding
- Trabalhando em servidor remoto
- Quer algo leve e rápido
- Debugging em ambiente headless

```bash
make        # Compila SEM SDL2 (apenas Terminal)
```

### Backend SDL2 (Para ambiente gráfico)
✅ **Use quando**:
- Trabalhando localmente com GUI
- X11/Wayland disponível
- Precisa de janelas gráficas
- Tem aceleração de hardware

```bash
make SDL2=1   # Compila COM SDL2
```

## 🔧 Detecção Automática vs Manual

### Automática (mqt_create_auto_backend)
O sistema detecta automaticamente o melhor backend:
```c
ctx = mqt_create_auto_backend(800, 400, "Minha App");
// Tenta SDL2 primeiro, fallback para Terminal
```

⚠️ **Problema**: Em SSH, SDL2 pode ser detectado mas falha ao criar janela

### Manual (mqt_create_backend) - RECOMENDADO
Força um backend específico:
```c
// Para SSH/Terminal
ctx = mqt_create_backend(MQT_BACKEND_TERMINAL, 80, 30, "App");

// Para GUI local
ctx = mqt_create_backend(MQT_BACKEND_SDL2, 800, 600, "App");
```

## 📦 Compilação

### Sem SDL2 (Terminal apenas)
```bash
cd examples
make
./basic_demo/basic_demo
```

### Com SDL2
```bash
cd examples
make SDL2=1
./basic_demo/basic_demo
```

### Limpar e recompilar
```bash
make fclean
make        # ou make SDL2=1
```

## 🐛 Troubleshooting

### "Janela SDL2 fecha imediatamente"
**Causa**: Você está em SSH sem X11 forwarding adequado

**Solução**: Use backend Terminal:
1. Recompile sem SDL2: `make fclean && make`
2. OU force Terminal no código (veja abaixo)

### "SDL_QUIT detectado logo após criar janela"
**Causa**: X server não consegue exibir janela (SSH remoto)

**Solução**: Compilar e rodar com Terminal:
```bash
cd examples/basic_demo
make fclean
make    # SEM SDL2=1
./basic_demo
```

### Forçar Backend Terminal no Código
Edite `main.c`:
```c
// Troque isto:
ctx = mqt_create_auto_backend(800, 400, "Demo");

// Por isto:
ctx = mqt_create_backend(MQT_BACKEND_TERMINAL, 80, 30, "Demo");
```

## 🖥️ Backends Disponíveis

| Backend | Ambiente | Performance | Interativo |
|---------|----------|-------------|------------|
| **Terminal** | SSH, TTY, Console | Muito rápido | Limitado (teclado) |
| **SDL2** | X11, Wayland, Desktop | Rápido (GPU) | Total (mouse, teclado) |
| **X11** | X11 direto | Médio | Total |

## ✅ Workflow Recomendado

### 1. Desenvolvimento em SSH (Servidor 42)
```bash
# Sempre compile SEM SDL2
make

# Execute exemplos
./basic_demo/basic_demo
./widget_demo/widget_demo
```

### 2. Teste Local (com GUI)
```bash
# Compile COM SDL2
make SDL2=1

# Execute com janela gráfica
./basic_demo/basic_demo
```

### 3. Debug
```bash
# Terminal sempre funciona
make
./basic_demo/basic_demo

# Se precisar de debug SDL:
make SDL2=1
SDL_DEBUG=1 ./basic_demo/basic_demo
```

## 📝 Notas Importantes

1. **SDL2 do sistema vs local**:
   - Makefiles usam `pkg-config` para detectar SDL2 do sistema
   - Se `lib/SDL2/install/` existir, usa versão local
   - Versão local tem prioridade

2. **Compilação condicional**:
   - Sem `SDL2=1`: Apenas código Terminal é compilado
   - Com `SDL2=1`: Ambos Terminal e SDL2 são incluídos
   
3. **Runtime**:
   - Backend é escolhido em runtime (não em compile time)
   - Mesmo compilando com SDL2, pode usar Terminal

4. **Tamanhos de tela**:
   - Terminal: Dimensões em caracteres (ex: 80x30)
   - SDL2: Dimensões em pixels (ex: 800x600)

## 🎨 Exemplos Prontos

Todos em `examples/`:

```bash
cd examples

# Compilar todos (Terminal)
make

# Ou com SDL2
make SDL2=1

# Executar
./basic_demo/basic_demo      # Formas e cores
./widget_demo/widget_demo    # Botões interativos
./simple_app/simple_app      # Template mínimo
```

## 🔗 Ver Também

- [README.md](../README.md) - Overview geral
- [ARCHITECTURE.md](../ARCHITECTURE.md) - Arquitetura do sistema
- [INTEGRATION.md](../INTEGRATION.md) - Integrar em seus projetos
