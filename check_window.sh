#!/bin/bash
# Test script para verificar se a janela do miniQT está aparecendo

echo "╔════════════════════════════════════════════════════════════╗"
echo "║     miniQT - Verificação de Janela SDL2                  ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Verifica se o processo está rodando
if ps aux | grep -q "[b]asic_demo"; then
    echo "✓ basic_demo está rodando"
    PID=$(ps aux | grep "[b]asic_demo" | awk '{print $2}')
    echo "  PID: $PID"
    echo ""
    
    echo "🔍 Verificações:"
    echo ""
    
    # Verifica conexão X11
    if xdotool search --name "MiniQT" 2>/dev/null; then
        echo "  ✓ Janela encontrada pelo xdotool!"
        WID=$(xdotool search --name "MiniQT" | head -1)
        echo "    Window ID: $WID"
        xdotool getwindowname $WID 2>/dev/null && echo ""
    else
        echo "  ⚠ xdotool não instalado ou janela não encontrada"
    fi
    
    # Lista todas as janelas X11
    if command -v wmctrl &> /dev/null; then
        echo "  📋 Janelas abertas:"
        wmctrl -l | head -10
    fi
    
    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo ""
    echo "❓ VOCÊ VÊ A JANELA 'MiniQT - Basic Demo'?"
    echo ""
    echo "   Se SIM:"
    echo "     🎉 Perfeito! O miniQT está funcionando!"
    echo "     → Pressione ESC na janela para fechar"
    echo "     → Ou execute: pkill basic_demo"
    echo ""
    echo "   Se NÃO:"
    echo "     → A janela pode estar em outro workspace/desktop virtual"
    echo "     → Verifique Alt+Tab para ver todas as janelas"
    echo "     → Ou execute: pkill basic_demo e tente novamente"
    echo ""
else
    echo "✗ basic_demo NÃO está rodando"
    echo ""
    echo "Execute primeiro:"
    echo "  cd /nfs/homes/lsarraci/my_shell/miniQT/examples/basic_demo"
    echo "  ./basic_demo &"
fi
