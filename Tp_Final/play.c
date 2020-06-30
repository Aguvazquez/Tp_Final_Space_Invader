

#define N 16
#define alien 
#define primera_fila 2
#define ultima_fila 7
#define primera_columna 2
#define ultima_columna 12
#define espacio_aliens 1
#define bloque 
#define fila_bloques N-2
#define primer_bloque 3
#define ultimo_bloque 12
#define espacio_bloques 3
#define nave

uint8_t play()
{
    uint8_t game[N][N], level=1;
    //while(condicion)
    {
        set_level(level);
        //print_level(game);
        //move();
        //logic();
    }
    return 0;
}

void set_level(uint8_t level)
{
    uint8_t i, j;
    for(i=primera_fila; i<=ultima_fila; i++)
    {
        for(j=primera_columna; j<=ultima_columna; j+=espacio_aliens+1)
            game[i][j] = alien;
    }
    
    if(level==1)    //los bloques solo se inicializan al principio
    {
        for(i=fila_bloques, j=primer_bloque; j<=ultimo_bloque; j+=espacio_bloques+1)
        {
            game[i][j++] = bloque;
            game[i][j] = bloque;
        }
    }
    
    //setear nave
}

void move()
{
    
}