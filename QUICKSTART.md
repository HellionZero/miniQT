# 🚀 MiniQT - Guia Rápido de Uso

## 📋 TL;DR

### Em Servidor SSH (42 School)
```bash
cd examples
make fclean && make    # SEM SDL2
./basic_demo/basic_demo
```

### Na Sua Máquina Local (com GUI)
```bash
cd examples
make fclean && make SDL2=1    # COM SDL2
./basic_demo/basic_demo
```

---

## 🖥️ Backend Terminal (SSH/Servidor)

### Compilar
```bash
cd /nfs/homes/lsarraci/my_shell/miniQT/examples
make fclean
make
```

### Executar
```bash
./basic_demo/basic_demo      # Formas e cores
./widget_demo/widget_demo    # Botões (limitado sem mouse)
./simple_app/simple_app      # Template básico
```

### O que esperar
- ✅ Renderização no terminal com ANSI colors
- ✅ Funciona via SSH
- ✅ Sem necessidade de X11/DISPLAY
- ⚠️ Sem suporte a mouse (apenas teclado)
- ⚠️ Resolução limitada pelo tamanho do terminal

---

## 🎨 Backend SDL2 (Máquina Local)

### Compilar
```bash
cd examples
make fclean
make SDL2=1
```

### Executar
```bash
./basic_demo/basic_demo
```

### O que esperar
- ✅ Janela gráfica completa
- ✅ Suporte total a mouse e teclado
- ✅ Aceleração por hardware (GPU)
- ✅ Resolução alta (pixels, não caracteres)
- ⚠️ Precisa de X11/Wayland/Windows/macOS

---

## 🔍 Verificar qual Backend está Compilado

```bash
cd examples/basic_demo
ldd basic_demo | grep SDL

# Se mostrar libSDL2: Compilado COM SDL2
# Se não mostrar nada: Compilado SEM SDL2 (Terminal apenas)
```

---

## 🐛 Troubleshooting

### "Janela fecha imediatamente" (SDL2)
**Problema**: Está em SSH sem X11 forwarding

**Solução 1**: Use backend Terminal
```bash
make fclean && make    # Sem SDL2=1
./basic_demo/basic_demo
```

**Solução 2**: SSH com X11 forwarding
```bash
# Na sua máquina local:
ssh -X usuario@servidor
cd miniQT/examples
make SDL2=1
./basic_demo/basic_demo
```

### "Caracteres estranhos no terminal"
**Problema**: Terminal não suporta ANSI colors

**Solução**: Use terminal moderno (não funciona em terminals muito antigos)

### "Nada acontece"
**Verificar**:
```bash
# O executável foi criado?
ls -lh basic_demo/basic_demo

# Executar com output:
./basic_demo/basic_demo
```

---

## 📦 Estrutura dos Exemplos

```
examples/
├── basic_demo/          # ← Comece aqui!
│   ├── main.c
│   ├── Makefile
│   └── basic_demo       (executável)
│
├── widget_demo/         # Botões interativos
│   ├── main.c
│   ├── Makefile
│   └── widget_demo
│
├── simple_app/          # Template mínimo
│   ├── main.c
│   ├── Makefile
│   └── simple_app
│
└── Makefile             # Compila tudo
```

---

## ⚡ Comandos Rápidos

```bash
# Limpar tudo
make fclean

# Compilar tudo (Terminal)
make

# Compilar tudo (SDL2)
make SDL2=1

# Compilar apenas basic_demo
cd basic_demo && make

# Ver opções
make help

# Recompilar do zero
make re
```

---

## 🎯 Quando Usar Cada Backend

### Use Terminal quando:
- ✅ Está em SSH
- ✅ Quer algo leve e rápido
- ✅ Não precisa de mouse
- ✅ Está debugando
- ✅ Servidor sem GUI

### Use SDL2 quando:
- ✅ Tem GUI disponível (X11/Wayland/Windows/macOS)
- ✅ Precisa de mouse
- ✅ Quer janelas bonitas
- ✅ Precisa de alta resolução
- ✅ Está na sua máquina local

---

## 🚀 Próximos Passos

1. **Teste agora (Terminal)**:
   ```bash
   cd examples
   make
   ./basic_demo/basic_demo
   ```

2. **Depois na sua máquina (SDL2)**:
   - Clone o projeto na sua máquina
   - Compile com `make SDL2=1`
   - Aproveite as janelas gráficas!

3. **Crie seu próprio projeto**:
   ```bash
   cp -r examples/simple_app meu_projeto
   cd meu_projeto
   # Edite main.c
   make
   ./simple_app
   ```

---

## 📚 Documentação Completa

- [README.md](README.md) - Overview do projeto
- [ARCHITECTURE.md](ARCHITECTURE.md) - Arquitetura detalhada
- [examples/COMPILE_GUIDE.md](examples/COMPILE_GUIDE.md) - Guia de compilação
- [INTEGRATION.md](INTEGRATION.md) - Usar em outros projetos

---

## 💡 Dica Final

Para desenvolvimento na 42:
```bash
# Sempre compile SEM SDL2
make

# Teste localmente
./basic_demo/basic_demo

# Commit e push
git add .
git commit -m "feat: add graphics"
git push

# Na sua casa, clone e:
make SDL2=1
./basic_demo/basic_demo  # Agora com janela gráfica!
```

**O código é o mesmo!** Apenas mude `make` ↔ `make SDL2=1`
