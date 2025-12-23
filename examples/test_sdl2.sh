#!/bin/bash
# Script para testar a janela SDL2 com máximo debug

export SDL_DEBUG=1
export SDL_VIDEO_X11_VISUALID=

echo "==================================="
echo "Teste SDL2 com Debug Máximo"
echo "==================================="
echo ""
echo "DISPLAY: $DISPLAY"
echo "User: $USER"
echo ""

cd /nfs/homes/lsarraci/my_shell/miniQT/examples/basic_demo

# Verificar se pode conectar no X
if ! xdpyinfo >/dev/null 2>&1; then
    echo "ERRO: Não consegue conectar no X server"
    exit 1
fi

echo "Executando basic_demo..."
echo ""

timeout 10 ./basic_demo 2>&1

echo ""
echo "==================================="
