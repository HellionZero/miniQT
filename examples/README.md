# 🎮 MiniQT - Exemplos e Testes

Esta pasta contém exemplos práticos de uso da API MiniQT.

## 📁 Estrutura

```
examples/
├── Makefile              # Makefile principal (compila todos)
├── basic_demo/           # Demo básico - formas e cores
│   ├── Makefile
│   └── main.c
├── widget_demo/          # Demo de widgets interativos
│   ├── Makefile
│   └── main.c
└── simple_app/           # Template mínimo para novos projetos
    ├── Makefile
    └── main.c
```

## 🚀 Como Compilar

### Compilar Todos os Exemplos

```bash
cd examples
make
```

### Compilar Exemplo Específico

```bash
# Demo básico
cd examples/basic_demo
make

# Demo de widgets
cd examples/widget_demo
make

# Aplicação simples
cd examples/simple_app
make
```

### Compilar com SDL2

```bash
cd examples
make SDL2=1

# Ou em exemplo específico
cd basic_demo
make SDL2=1
```

## ▶️ Como Executar

### Executar diretamente

```bash
# Basic Demo
./basic_demo/basic_demo

# Widget Demo
./widget_demo/widget_demo

# Simple App
./simple_app/simple_app
```

### Executar com make

```bash
cd basic_demo
make run

# Com SDL2
make SDL2=1 run
```

## 📚 Descrição dos Exemplos

### 1. **basic_demo** - Demonstração Básica
Mostra o uso fundamental da API sem interação.

**Features:**
- Desenho de retângulos coloridos
- Renderização de texto
- Paleta de cores
- Backend automático

**Ideal para:**
- Primeiros passos com a API
- Entender a estrutura básica
- Testar backends

**Código mínimo:**
```c
ctx = mqt_create_auto_backend(800, 400, "Demo");
mqt_begin_frame(ctx);
mqt_draw_rectangle_filled(ctx, rect, color);
mqt_end_frame(ctx);
mqt_backend_destroy(ctx);
```

---

### 2. **widget_demo** - Widgets Interativos
Demonstra sistema de widgets com botões interativos.

**Features:**
- Botões com estados (normal, hover, pressed)
- Callbacks de clique
- Habilitação/desabilitação dinâmica
- Interação com mouse
- Cores customizáveis

**Ideal para:**
- Aprender sistema de widgets
- Entender event handling
- Criar UIs interativas

**Código chave:**
```c
t_button *btn = mqt_button_create("OK", 100, 100, 150, 50);
btn->on_click = callback_function;

// No loop
mqt_button_update(btn, mouse_x, mouse_y, mouse_down);
mqt_button_draw(ctx, btn);
```

---

### 3. **simple_app** - Template Mínimo
Aplicação mais simples possível - ponto de partida para projetos.

**Features:**
- Código mínimo funcional
- Estrutura clara
- Comentários explicativos

**Ideal para:**
- Começar um novo projeto
- Entender estrutura básica
- Copiar e modificar

**Use como base:**
```bash
cp -r examples/simple_app meu_projeto
cd meu_projeto
# Edite main.c conforme necessário
make
./simple_app
```

## 🎯 Guia Rápido

### Criar Novo Exemplo

```bash
# 1. Copiar template
cp -r simple_app meu_exemplo

# 2. Editar Makefile
cd meu_exemplo
# Mudar NAME = meu_exemplo

# 3. Editar main.c
# Implementar sua lógica

# 4. Compilar
make

# 5. Executar
./meu_exemplo
```

### Adicionar ao Build Principal

Edite `examples/Makefile`:
```makefile
all: basic widget simple meu_exemplo

meu_exemplo:
	@make -C meu_exemplo
```

## 🔧 Dependências

Todos os exemplos dependem de:
- **libft** (compilado automaticamente)
- **ft_printf** (compilado automaticamente)
- **miniqt_backend** (incluído no build)
- **miniqt_render** (incluído no build)
- **(Opcional) SDL2** - Para backend gráfico

## 🐛 Troubleshooting

### Erros de Compilação

```bash
# Limpar tudo e recompilar
make fclean
make

# Limpar libs principais também
cd ..
make fclean
cd examples
make
```

### SDL2 não encontrado

```bash
# Executar setup SDL2 na raiz do projeto
cd ..
./setup_sdl2.sh
cd examples
make SDL2=1
```

### Exemplo não executa

```bash
# Verificar backend disponível
./basic_demo/basic_demo
# Leia a saída - mostra qual backend está sendo usado

# Forçar backend terminal
# (edite main.c e use mqt_create_backend(MQT_BACKEND_TERMINAL, ...))
```

## 📝 Controles

### Todos os exemplos:
- **ESC** - Sair
- **Fechar janela (X)** - Sair (apenas SDL2)

### Widget Demo:
- **Mouse** - Interagir com botões
- **Click** - Acionar callbacks

## 💡 Dicas de Desenvolvimento

### Performance

```c
// Terminal: ~10-30 FPS
if (ctx->backend_type == MQT_BACKEND_TERMINAL) {
    sleep(3);  // Renderiza uma vez e sai
}

// SDL2: 60 FPS
else {
    usleep(16666);  // ~60 FPS
}
```

### Debug

```c
// Listar backends disponíveis
mqt_list_available_backends();

// Verificar qual está sendo usado
printf("Backend: %s\n", mqt_backend_name(ctx->backend_type));
```

### Cores

```c
// Definir paleta no início
t_color bg = mqt_color(30, 30, 40, 255);
t_color primary = mqt_color(80, 120, 200, 255);
t_color text = mqt_color(255, 255, 255, 255);
```

## 🔗 Próximos Passos

1. Execute os exemplos
2. Leia o código fonte
3. Modifique e experimente
4. Crie seu próprio exemplo
5. Consulte `../INTEGRATION.md` para usar em projetos externos

## 📖 Documentação Adicional

- [../README.md](../README.md) - Overview do projeto
- [../ARCHITECTURE.md](../ARCHITECTURE.md) - Arquitetura detalhada
- [../SECURITY_AND_FEATURES.md](../SECURITY_AND_FEATURES.md) - Segurança e features
- [../INTEGRATION.md](../INTEGRATION.md) - Como integrar em outros projetos

## 🎨 Próximos Exemplos (TODO)

- [ ] `login_form/` - Formulário com inputs
- [ ] `menu_system/` - Sistema de menus
- [ ] `snake_game/` - Jogo Snake simples
- [ ] `file_browser/` - Navegador de arquivos
- [ ] `terminal_dashboard/` - Dashboard estilo ncurses
