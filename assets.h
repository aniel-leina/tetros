#ifndef ASSETS_H
#define ASSETS_H

#define BLOCK  "██"

// --- COLORES BASE (LAS 7 PIEZAS DEL TETRIS) ---
#define RESET     "\x1b[0m"

#define CIAN      "\x1b[36m"       // Pieza I
#define AZUL      "\x1b[34m"       // Pieza J
#define NARANJA   "\x1b[38;5;208m" // Pieza L (Paleta 256 colores para naranja real)
#define AMARILLO  "\x1b[33m"       // Pieza O
#define VERDE     "\x1b[32m"       // Pieza S
#define MORADO    "\x1b[35m"       // Pieza T (Técnicamente es Magenta)
#define ROJO      "\x1b[31m"       // Pieza Z

// --- EXTRAS 1: COLORES BRILLANTES ---
// Si tu terminal tiene poco contraste o usas un tema muy oscuro, estos se ven más vivos
// --- COLORES DE PIEZAS BRILLANTES (Alta Intensidad) ---
#define CIAN_B      "\x1b[96m"       // Pieza I
#define AZUL_B      "\x1b[94m"       // Pieza J
#define NARANJA_B   "\x1b[38;5;214m" // Pieza L (Naranja más eléctrico/vivo)
#define AMARILLO_B  "\x1b[93m"       // Pieza O
#define VERDE_B     "\x1b[92m"       // Pieza S
#define MORADO_B    "\x1b[95m"       // Pieza T
#define ROJO_B      "\x1b[91m"       // Pieza Z
#define BLANCO_B    "\x1b[97m"       // (Opcional, útil para textos o la UI)
// --- EXTRAS 2: COLORES DE FONDO (BACKGROUND) ---
// Ideales para pintar las paredes del pozo o un menú
#define BG_GRIS     "\x1b[100m" // Fondo gris oscuro para la cuadrícula vacía
#define BG_BLANCO   "\x1b[47m"  // Fondo blanco sólido para los límites
#define BG_AZUL     "\x1b[44m"  // Fondo azul (útil para decorar)

// --- EXTRAS 3: ESTILOS ---
#define NEGRITA     "\x1b[1m"   // Hace que los caracteres destaquen más
#define PARPADEO    "\x1b[5m"   // Texto parpadeante (¡Genial para el mensaje de GAME OVER!)

#endif
