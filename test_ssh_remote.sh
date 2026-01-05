#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_ssh_remote.sh                                 :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsarraci <lsarraci@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/02 00:00:00 by lsarraci          #+#    #+#              #
#    Updated: 2026/01/02 00:00:00 by lsarraci         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Script para testar miniQT em ambientes SSH remotos
# Testa diferentes configurações de display e X11 forwarding

COLOR_RESET="\033[0m"
COLOR_GREEN="\033[32m"
COLOR_YELLOW="\033[33m"
COLOR_RED="\033[31m"
COLOR_BLUE="\033[34m"

echo -e "${COLOR_BLUE}═══════════════════════════════════════════════════════════${COLOR_RESET}"
echo -e "${COLOR_BLUE}  MiniQT - Teste de Ambiente SSH Remoto${COLOR_RESET}"
echo -e "${COLOR_BLUE}═══════════════════════════════════════════════════════════${COLOR_RESET}"
echo

# Verifica se está em sessão SSH
if [ -n "$SSH_CONNECTION" ] || [ -n "$SSH_CLIENT" ] || [ -n "$SSH_TTY" ]; then
    echo -e "${COLOR_GREEN}✓${COLOR_RESET} Sessão SSH detectada"
    echo "  SSH_CONNECTION: $SSH_CONNECTION"
    echo "  SSH_TTY: $SSH_TTY"
else
    echo -e "${COLOR_YELLOW}⚠${COLOR_RESET} Não está em sessão SSH (executando localmente)"
fi
echo

# Verifica variáveis de ambiente relevantes
echo -e "${COLOR_BLUE}Variáveis de Ambiente:${COLOR_RESET}"
echo "  DISPLAY: ${DISPLAY:-não definido}"
echo "  WAYLAND_DISPLAY: ${WAYLAND_DISPLAY:-não definido}"
echo "  XDG_SESSION_TYPE: ${XDG_SESSION_TYPE:-não definido}"
echo

# Verifica X11 forwarding
if [ -n "$DISPLAY" ]; then
    echo -e "${COLOR_GREEN}✓${COLOR_RESET} DISPLAY está configurado: $DISPLAY"
    
    # Tenta verificar se X11 está realmente funcionando
    if command -v xdpyinfo &> /dev/null; then
        if xdpyinfo &> /dev/null; then
            echo -e "${COLOR_GREEN}✓${COLOR_RESET} X11 forwarding está funcionando!"
        else
            echo -e "${COLOR_RED}✗${COLOR_RESET} DISPLAY configurado mas X11 não responde"
            echo -e "${COLOR_YELLOW}  Dica: Conecte com 'ssh -X' ou 'ssh -Y'${COLOR_RESET}"
        fi
    else
        echo -e "${COLOR_YELLOW}⚠${COLOR_RESET} xdpyinfo não disponível (não é possível testar X11)"
    fi
else
    echo -e "${COLOR_RED}✗${COLOR_RESET} DISPLAY não está configurado"
    echo -e "${COLOR_YELLOW}  Dica: Use 'ssh -X usuario@servidor' para habilitar X11 forwarding${COLOR_RESET}"
fi
echo

# Verifica se SDL2 está disponível
echo -e "${COLOR_BLUE}Verificando Dependências:${COLOR_RESET}"
if pkg-config --exists sdl2 2>/dev/null; then
    SDL2_VERSION=$(pkg-config --modversion sdl2)
    echo -e "${COLOR_GREEN}✓${COLOR_RESET} SDL2 instalado (versão $SDL2_VERSION)"
else
    echo -e "${COLOR_RED}✗${COLOR_RESET} SDL2 não encontrado"
    echo -e "${COLOR_YELLOW}  Execute: ./setup_sdl2.sh${COLOR_RESET}"
fi
echo

# Testa compilação
echo -e "${COLOR_BLUE}Testando Compilação:${COLOR_RESET}"
if [ -f "Makefile" ]; then
    echo "Compilando miniQT..."
    if make clean && make; then
        echo -e "${COLOR_GREEN}✓${COLOR_RESET} Compilação bem-sucedida"
    else
        echo -e "${COLOR_RED}✗${COLOR_RESET} Erro na compilação"
        exit 1
    fi
else
    echo -e "${COLOR_RED}✗${COLOR_RESET} Makefile não encontrado"
    exit 1
fi
echo

# Recomendações
echo -e "${COLOR_BLUE}═══════════════════════════════════════════════════════════${COLOR_RESET}"
echo -e "${COLOR_BLUE}  Recomendações para Conexão SSH:${COLOR_RESET}"
echo -e "${COLOR_BLUE}═══════════════════════════════════════════════════════════${COLOR_RESET}"
echo
echo -e "${COLOR_GREEN}Para janelas SDL2 (GUI):${COLOR_RESET}"
echo "  ssh -X usuario@servidor          # X11 forwarding normal"
echo "  ssh -Y usuario@servidor          # X11 forwarding confiável"
echo "  ssh -XC usuario@servidor         # Com compressão (mais rápido)"
echo
echo -e "${COLOR_GREEN}Para testar no servidor remoto:${COLOR_RESET}"
echo "  cd miniQT"
echo "  make"
echo "  ./examples/basic_demo/basic_demo       # Usa SDL2 se DISPLAY disponível"
echo "  ./examples/widget_demo/widget_demo     # Ou fallback para terminal"
echo
echo -e "${COLOR_YELLOW}Troubleshooting:${COLOR_RESET}"
echo "  1. Se X11 forwarding não funcionar:"
echo "     - No servidor: edite /etc/ssh/sshd_config"
echo "     - Adicione: X11Forwarding yes"
echo "     - Reinicie SSH: sudo systemctl restart sshd"
echo
echo "  2. Se aparecer erro 'cannot open display':"
echo "     - export DISPLAY=localhost:10.0"
echo "     - Ou reconecte com: ssh -X"
echo
echo "  3. Sem X11 (terminal puro):"
echo "     - A aplicação usa automaticamente backend de terminal"
echo "     - Funciona em qualquer SSH sem configuração especial"
echo

# Executa exemplo se possível
echo -e "${COLOR_BLUE}═══════════════════════════════════════════════════════════${COLOR_RESET}"
echo -e "${COLOR_BLUE}  Executando Exemplo:${COLOR_RESET}"
echo -e "${COLOR_BLUE}═══════════════════════════════════════════════════════════${COLOR_RESET}"
echo

if [ -f "examples/basic_demo/basic_demo" ]; then
    echo "Iniciando basic_demo..."
    echo "Pressione Ctrl+C para sair"
    echo
    ./examples/basic_demo/basic_demo
else
    echo -e "${COLOR_YELLOW}Compile os exemplos primeiro:${COLOR_RESET}"
    echo "  cd examples && make"
fi
