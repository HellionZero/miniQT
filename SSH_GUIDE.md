# 🌐 Guia de Uso em Ambiente SSH Remoto

Este guia explica como usar o miniQT em servidores remotos via SSH, especialmente útil para ambientes como a 42.

## 🔧 Configuração Rápida

### 1. Conectar com X11 Forwarding

```bash
# Método recomendado (com compressão)
ssh -XC usuario@servidor.42.fr

# Alternativa mais segura
ssh -Y usuario@servidor.42.fr

# Verificar se DISPLAY está configurado
echo $DISPLAY
# Deve mostrar algo como: localhost:10.0
```

### 2. Compilar no Servidor

```bash
cd miniQT
make clean && make

# Compilar exemplos
cd examples && make
```

### 3. Executar Aplicação

```bash
# A aplicação detecta automaticamente o ambiente
./examples/basic_demo/basic_demo

# Você verá uma mensagem como:
# [MiniQT] Platform: Linux
# [MiniQT] SSH session detected
# [MiniQT] X11 forwarding: active
# [MiniQT] Using backend: SDL2
```

## 🎯 Como Funciona

### Detecção Automática de Ambiente

O miniQT detecta automaticamente se está rodando via SSH:

```c
// Verifica variáveis de ambiente
SSH_CONNECTION    // Indica conexão SSH ativa
SSH_TTY          // Terminal SSH
DISPLAY          // X11 forwarding configurado
```

### Modos de Operação

1. **SSH + X11 Forwarding** → Janela SDL2 no seu computador local
2. **SSH sem X11** → Interface de terminal (TUI)
3. **Local** → Janela SDL2 nativa

## 📋 Script de Teste

Use o script fornecido para verificar a configuração:

```bash
./test_ssh_remote.sh
```

O script verifica:
- ✓ Detecção de sessão SSH
- ✓ Variáveis de ambiente (DISPLAY, etc.)
- ✓ Status do X11 forwarding
- ✓ Disponibilidade do SDL2
- ✓ Compilação e execução de exemplo

## 🔒 Segurança e Isolamento

### Características de Segurança

- **Nenhum arquivo de configuração**: Não modifica `/etc`, `~/.config`, etc.
- **Sessão temporária**: Tudo é destruído ao fechar o SSH
- **Sem privilégios**: Não requer `sudo` ou permissões especiais
- **Isolado**: Cada sessão SDL2 é independente

### Limpeza Automática

```bash
# Ao desconectar do SSH, automaticamente:
# - SDL_Quit() é chamado
# - Janelas são fechadas
# - Nenhum processo fica rodando
# - Nenhuma configuração permanece
```

## 🚀 Casos de Uso

### Cenário 1: Desenvolvimento na 42

```bash
# No seu Mac/Linux/Windows
ssh -XC login@c1r1s1.42.fr

# No servidor da 42
cd ~/miniQT
make
./examples/widget_demo/widget_demo
# A janela aparece no seu computador!
```

### Cenário 2: Servidor sem GUI

```bash
# SSH normal (sem -X)
ssh login@servidor.42.fr

# No servidor
cd ~/miniQT
./examples/basic_demo/basic_demo
# Usa automaticamente o backend de terminal (TUI)
```

### Cenário 3: Múltiplas Janelas

```bash
# Cada janela é uma sessão SDL independente
./app1 &  # Primeira janela
./app2 &  # Segunda janela
# Ambas aparecem no seu computador local
```

## 🛠️ Troubleshooting

### Problema: "cannot open display"

```bash
# Solução 1: Reconectar com X11
exit
ssh -X usuario@servidor

# Solução 2: Configurar DISPLAY manualmente
export DISPLAY=localhost:10.0

# Solução 3: Verificar configuração do servidor
cat /etc/ssh/sshd_config | grep X11Forwarding
# Deve mostrar: X11Forwarding yes
```

### Problema: Janela muito lenta

```bash
# Use compressão SSH
ssh -XC usuario@servidor

# Ou reduza a qualidade gráfica no código
# (As operações de desenho já são otimizadas)
```

### Problema: X11 forwarding bloqueado

```bash
# Se o servidor não permite X11 forwarding:
# A aplicação automaticamente usa o backend de terminal
# Nenhuma ação necessária!
```

## 📊 Performance

### Otimizações para SSH

- **Compressão**: Use `ssh -C` para reduzir tráfego de rede
- **KeepAlive**: Evita desconexões em longas sessões
- **Multiplexing**: Reutiliza conexões SSH

Exemplo de `~/.ssh/config`:

```
Host servidor42
    HostName c1r1s1.42.fr
    User seu_login
    ForwardX11 yes
    ForwardX11Trusted yes
    Compression yes
    ServerAliveInterval 60
    ServerAliveCountMax 3
    ControlMaster auto
    ControlPath ~/.ssh/cm-%r@%h:%p
    ControlPersist 10m
```

Uso:
```bash
ssh servidor42
# X11 forwarding automático com todas as otimizações!
```

## 🎓 Exemplos Práticos

### Exemplo 1: Teste Simples

```bash
# Conectar
ssh -XC login@servidor.42.fr

# Testar
cd ~/miniQT
./test_ssh_remote.sh

# Se tudo OK, executar
./examples/basic_demo/basic_demo
```

### Exemplo 2: Desenvolvimento Iterativo

```bash
# Em uma janela SSH
ssh -XC login@servidor.42.fr
cd ~/miniQT

# Editar código
vim src/example.c

# Compilar e testar
make && ./examples/basic_demo/basic_demo

# Repetir ciclo edit-compile-test
```

### Exemplo 3: Debug Remoto

```bash
# SSH com X11
ssh -XC login@servidor.42.fr

# Executar com debug
cd ~/miniQT
make re
gdb ./examples/basic_demo/basic_demo

# No GDB
(gdb) run
# Janela aparece normalmente, você pode debugar remotamente!
```

## ✨ Recursos Avançados

### Fallback Automático

```c
// O código já implementa fallback inteligente:
// 1. Tenta SDL2 com X11 forwarding
// 2. Se falhar, usa backend de terminal
// 3. Sempre funciona, independente do ambiente
```

### Detecção de SSH

```c
// Verifica automaticamente:
if (getenv("SSH_CONNECTION"))
    printf("Rodando via SSH\n");

// Otimiza para ambiente remoto
if (ssh_session && x11_forwarding)
    use_optimized_rendering();
```

## 📝 Notas Importantes

1. **X11 Forwarding é seguro**: Usa criptografia SSH
2. **Não afeta o servidor**: Renderização ocorre localmente
3. **Bandwidth**: Depende da complexidade gráfica
4. **Latência**: Melhor em redes locais (LAN)

## 🔗 Links Úteis

- [OpenSSH X11 Forwarding](https://www.ssh.com/academy/ssh/x11-forwarding)
- [SDL2 Documentation](https://wiki.libsdl.org/)
- [miniQT Architecture](ARCHITECTURE.md)

---

**Dica**: Para melhor experiência, use `ssh -XC` sempre que possível!
