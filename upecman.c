
/*
 * Instrucoes para compilar:
 *      $make upecman
 */

/* ---------------------------------------------------------------------- */
/* includes */

#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Miscellaneous functions (rand, malloc, srand)*/
#include <ncurses.h> /* Screen handling and optimisation functions */
#include <getopt.h> /* get options from system argc/argv */
#include <string.h> /* Strings functions definitions */
#include <assert.h> /* Verify assumptions with assert */
#include <unistd.h> /* UNIX standard function */
#include "upecman.h" /* To be created for this template if needed */
#include <time.h>  /* Time and date functions */
#include <math.h>  /* Mathematics functions */

 main(int argc, char *argv[])
{

    IFDEBUG("main()\n");
    int opt; /* return from getopt() */
    t_game g; /* the game */

    IFDEBUG("Starting optarg loop...\n");

    /* getopt() configured options:
     *        -h  help
     *        -c  copyright & version
     *        -v  verbose
     */
    opterr = 0;
    while((opt = getopt(argc, argv, "hvc")) != EOF)
        switch(opt)
        {
            case 'h':
                help();
                break;
            case 'v':
                verb++;
                break;
            case 'c':
                copyr();
                break;
            case '?':
            default:
                printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return EXIT_FAILURE;
        }



    if(verb)
        printf("Verbose level set at: %d\n", verb);
    IFDEBUG("Starting the game now...\n");

    g = upecman_init();
    //pre_game(g);
    menu(g);

    endwin();
    return EXIT_SUCCESS;
}

/* ---------------------------------------------------------------------- */

void help(void)
{
    IFDEBUG("help()");
    printf("%s - %s\n", "upecman", "Pacman Ncurses");
    printf("\nUsage: ./%s [-h|-v|-c]\n", "upecman");
    printf("\nOptions:\n");
    printf("\t-h,  --help\n\t\tShow this help.\n");
    printf("\t-c,  --copyright, --version\n\t\tShow version and copyright information.\n");
    printf("\t-v,  --verbose\n\t\tSet verbose level (cumulative).\n");
    /* add more options here */
    printf("\nExit status:\n\t0 if ok.\n\t1 some error occurred.\n");
    printf("\nTodo:\n\tLong options not implemented yet.\n");
    printf("\nAuthor:\n\tWritten by %s <%s>\n\n", "Ruben Carlo Benante", "rcb@beco.cc");
    exit(EXIT_FAILURE);
}


 */
void copyr(void)
{
    IFDEBUG("copyr()");
    printf("%s - Version %s\n", "upecman", VERSION);
    printf("\nCopyright (C) %d %s <%s>, GNU GPL version 2 <http://gnu.org/licenses/gpl.html>. This  is  free  software:  you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law. USE IT AS IT IS. The author takes no responsability to any damage this software may inflige in your data.\n\n", 2016, "Ruben Carlo Benante", "rcb@beco.cc");
    if(verb > 3) printf("copyr(): Verbose: %d\n", verb); /* -vvvv */
    exit(EXIT_FAILURE);
}

t_game upecman_init(void)
{
    IFDEBUG("init()");
    /* initialization */

    t_game g;
    int f, y;

    for(y = 0; y < LABL; y++)
        strcpy(g.lab[y], labmodel[y]); /* copy the labyrinth (lines of strings) */

    g.pacman.pos.y = 17; /* pacman start position (line) */
    g.pacman.pos.x = 9; /* pacman start position (column) */
    g.pacman.dir = left; /* pacman start direction */
    g.pacman.life = 3; /* 3 lifes */
    g.pacman.score = 0; /* 0 points */
    g.pacman.pacdots = 0;
    g.pacman.pill = 0;
    g.pacman.tempocereja = 0;
    g.pacman.cherry = 0;

    g.lab[7][9] =  ' ';
    g.lab[9][10] =  ' ';
    g.lab[10][10] =  ' ';
    g.lab[11][10] =  ' ';
    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
    {
        switch(f)
        {
            case blinky:
                g.ghost[f].pos.y = 7; /* blinky start position (line) */
                g.ghost[f].pos.x = 9; /* blinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case pinky:
                g.ghost[f].pos.y = 9; /* pinky start position (line) */
                g.ghost[f].pos.x = 10; /* pinky start position (column) */
                g.ghost[f].starget.y = 0; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
                break;
            case inky:
                g.ghost[f].pos.y = 10; /* inky start position (line) */
                g.ghost[f].pos.x = 10; /* inky start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 19; /* preferred corner (column) */
                break;
            case clyde:
                g.ghost[f].pos.y = 11; /* clyde start position (line) */
                g.ghost[f].pos.x = 10; /* clyde start position (column) */
                g.ghost[f].starget.y = 22; /* preferred corner (line) */
                g.ghost[f].starget.x = 0; /* preferred corner (column) */
        }
        g.ghost[f].dir = left; /* start direction and future direction: left */
        g.ghost[f].mode = chase;
    }

    initscr(); /* start ncurses mode screen */
    cbreak(); /* stop line buffering */
    keypad(stdscr, TRUE); /* grab extended keys */
    noecho(); /* stop echoing chars */
    timeout(0); /* do not wait */
    start_color(); /* initialize the color functionality */

    /* Initialize all the colors */
    init_pair(1, COLOR_RED, COLOR_BLACK); /* Blinky */
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK); /* Pinky */
    init_pair(3, COLOR_CYAN, COLOR_BLACK); /* Inky */
    init_pair(4, COLOR_GREEN, COLOR_BLACK); /* Clyde doesn't have orange :( */
    init_pair(5, COLOR_BLUE, COLOR_BLACK); /* Afraid mode: blue with A_BLINK */
    init_pair(6, COLOR_YELLOW, COLOR_BLACK); /* Pacman */
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_BLUE, COLOR_BLUE);
    init_pair(9, COLOR_WHITE, COLOR_YELLOW);
    init_pair(10, COLOR_BLACK, COLOR_RED);
    init_pair(11, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(12, COLOR_BLUE, COLOR_WHITE);
    return g;
}


void printlab(t_game g)
{
    clear();
    initscr();
    IFDEBUG("printlab()");

    int y, f, x;
    /*clear(); / * clear the screen */
    for(y = 0; y < LABL; y++)
        printw("%s\n", g.lab[y]);
    mvprintw(y , 18, "%%");
    mvchgat(y, 18, 1, A_BOLD, 1, NULL);
    for(f = blinky; f <= clyde; f++) /* blinky, pinky, inky and clyde */
        mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, f + 1, NULL);
    for(y = 0; y < LABL; y++)
        for(x = 0; x < LABC; x++)
            if(g.lab[y][x] == '#')
            {
                mvprintw(y, x, " ");
                mvchgat(y, x, 1, A_BOLD, 8, NULL);
            }
    mvchgat(g.pacman.pos.y, g.pacman.pos.x, 1, A_BOLD, 6, NULL);
    art();
    refresh();
}
void art(void)
{
    int x, y;
    for(y = 0; y < DSY; y++)
    {
        for(x = 0; x < DSX; x++)
        {
            mvprintw(5 + y, 30 + x, " ");
            if(g1[y][x] == 'p')
                mvchgat(5 + y, 30 + x, 1, A_BOLD, 11, NULL);
            if(g1[y][x] == 'w')
                mvchgat(5 + y, 30 + x, 1, A_BOLD, 12, NULL);
            if(g1[y][x] == 'b')
                mvchgat(5 + y, 30 + x, 1, A_BOLD, 8, NULL);

            mvprintw(5 + y, 45 + x, " ");
            if(g1[y][x] == 'p')
                mvchgat(5 + y, 45 + x, 1, A_BOLD, 10, NULL);
            if(g1[y][x] == 'w')
                mvchgat(5 + y, 45 + x, 1, A_BOLD, 12, NULL);
            if(g1[y][x] == 'b')
                mvchgat(5 + y, 45 + x, 1, A_BOLD, 8, NULL);

            mvprintw(5 + y, 60 + x, " ");
            if(pac[y][x] == 'p')
             mvchgat(5 + y, 60 + x, 1, A_BOLD, 9, NULL);

            mvprintw(5 + y, 75 + x, " ");
            if(g2[y][x] == 'p')
                mvchgat(5 + y, 75 + x, 1, A_BOLD, 8, NULL);
            if(g2[y][x] == '\\' || g2[y][x] == '/')
            {
                mvprintw(5 + y, 75 + x, "%c", g2[y][x]);
                mvchgat(5 + y, 75 + x, 1, A_BOLD, 7, NULL);
            }
            if(g2[y][x] == '0')
                mvchgat(5 + y, 75 + x, 1, A_BOLD, 12, NULL);
        }
    }

    return;
}
void regras()
{
    int ch;
    int  maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);
    WINDOW *regras_win = newwin(maxy, maxx, 0, 0);
    initscr();
    noecho();
    cbreak();
    box(regras_win, 0, 0);

    mvwprintw(regras_win, 3, 1, "Tutorial do pacman:");
    mvwprintw(regras_win, 5, 1, "1 - Coma o máximo de pelotas que conseguir, cada pelota consederá 10 pontos, há 184 pelotas distribuidas pelo mapa");
    mvwprintw(regras_win, 7, 1, "2 - Você tem 3 vidas por jogo");
    mvwprintw(regras_win, 9, 1, "3 - Os fantasmas tem 4 modos o modo chase no qual ele persegue o pacman, o modo scatter no qual eles se dispersam pelo mapa, modo afraid no qual eles se movem de forma ");
    mvwprintw(regras_win, 11, 1, "aleatória pelo mapa e o modo dead onde caso eles sejam comidos pelo pacman eles voltam à sua casa");
    mvwprintw(regras_win, 13, 1, "4 - No jogo ha 4 pastilhoes espalhados pelo mapa caso você consiga pega-los você consgiguira temporariamente comer os fantasmas ganhando assim 750 pontos por fantasma, além disso o jogador ganha 50 pontos por pastilhão");
    mvwprintw(regras_win, 15, 1, "5 - No jogo a cada 70 pelotas comidas ira spawnar no mapa uma cereja a cada cereja comida o usuário ganhará 500 pontos");
    mvwprintw(regras_win, 17, 1, "6 - o jogo tem áreas nas quais os fantasmas não podem subir use isso ao seu favor para escapar deles");
    mvwprintw(regras_win, 19, 1, "7 - o jogo tem 2 portais os quais são muito úteis para fugir dos fantasmas ja que ao chegar perto deles os fantasmas ficam mais lentos, além de ser uma forma rápida de se mover pelo mapa");
    mvwprintw(regras_win, 21, 1, "8 -  o jogo tem 3 diferentes níveis a cada nível a velocidade do jogo vai aumentando devido ao aumento da velocidade dos fantasmas, e por consequência a dificuldade vai aumentando");
    mvwprintw(regras_win, 23, 1, "9 - Para ganhar o jogo, o jogador deverá vencer os 3 níveis do jogo, para vence-los o jogador deve pegar as pelotas espalhadas pelo mapa sem perder todas suas vidas, caso perca as 3 vidas o jogador pederá o jogo");
    mvwprintw(regras_win, 27, 1, "- MOVIMENTACAO DO PACMAN:");
    mvwprintw(regras_win, 28, 1, "         _____ ");
    mvwprintw(regras_win, 29, 1, "        |  ^  |");
    mvwprintw(regras_win, 30, 1, "        |  |  |");
    mvwprintw(regras_win, 31, 1, "   _____|_____|_____ ");
    mvwprintw(regras_win, 32, 1, "  |     |  |  |     |");
    mvwprintw(regras_win, 33, 1, "  | <-  |  v  |  -> |");
    mvwprintw(regras_win, 34, 1, "  |_____|_____|_____|");
    mvwprintw(regras_win, 40, 1, "                                _____ ");
    mvwprintw(regras_win, 41, 1, "                               |     |");
    mvwprintw(regras_win, 42, 1, "- Para voltar ao menu, aperte: |  Q  |");
    mvwprintw(regras_win, 43, 1, "                               |_____|");


    wrefresh(regras_win);
    while ((ch = getch()) != 'q')
        usleep(16667); /* espere 16666 microseconds para evitar fritar o processador (1/60s) */
    werase(regras_win);
    wrefresh(regras_win);
    delwin(regras_win);
    endwin();
}
/*=======================================================================================
* MENU
*/
void menu(t_game g)
{
    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);
    int cont = 0, rep = 0;
    int i;
    int escolha;
    int quit = 0;
    int destaque = 0;
    int ymax, xmax;
    char opcoes[3][12] = {"jogar", "como jogar?", "sair"};
    getmaxyx (stdscr, ymax, xmax);
    WINDOW *janela = newwin(8, (xmax- 15), (ymax- 10), 9);
    while(1)
    {
        wrefresh(janela);
        keypad(janela, true);
        box(janela, 0, 0);
        rep++;
        if(rep % 10 == 0)
            cont++;
        usleep(16667); /* espere 16666 microseconds para evitar fritar o processador (1/60s) */
        logo(cont);
        for(i = 0; i < 3; i++)
        {
            mvwprintw(janela, 1, 1, "     Escolha uma opcao:");
            mvwprintw(janela, 2, 1, "   ");
            if(i  == destaque)
                wattron(janela, A_REVERSE);
            mvwprintw(janela, i + 3, (ymax / 3)- 7, "%s", opcoes[i]);
            wattroff (janela, A_REVERSE);
        }
        wrefresh(janela);
        escolha = getch();
        switch(escolha)
        {
            case KEY_UP:
                if(destaque > 0)
                    destaque --;
                break;

            case KEY_DOWN:
                if (destaque < 2)
                    destaque ++;
                break;
            case 10:
                escolha = destaque;

        }
        switch (escolha)
        {
            case 0:
                clear();
                pre_game(g);
                clear();
                initscr();
                break;
            case 1:
                regras();
                clear();
                initscr();
                break;;
            case 2:
                quit = 1;
                break;
        }
        if(quit == 1)
            break;
    }
    return;
}

void logo(int cont)
{
    int y, x, ymax, xmax, newx;
    getmaxyx (stdscr, ymax, xmax);
    newx = (xmax/2) - (DX/2);
    for(y = 0; y < DY; y++)
    {
        for(x = 0; x < DX; x++)
        {
            if(y > 17)
            {
                mvprintw(y + 2 + ymax/5, x + newx, "%c" ,title[y][x]);
                mvchgat(y + 2 + ymax/5, x + newx, 1, A_BOLD, 3, NULL);
            }
            else
            {
                mvprintw(y + ymax/5, x + newx, " ");
                if(title[y][x] == '.')
                    mvchgat(y + ymax/5, x + newx, 1, A_BOLD, 12, NULL);
                else
                    if(title[y][x] == ',')
                        mvchgat(y + ymax/5, x + newx, 1, A_BOLD, 9, NULL);
                    else
                        if(title[y][x] == 'x')
                            if(cont % 2 == 0)
                                mvchgat(y + ymax/5, x + newx, 1, A_BOLD, 9, NULL);
            }
        }
    }
    refresh();
    return;
}
/* ---------------------------------------------------------------------- */
/* Chama Looping principal do jogo */
void pre_game(t_game g)
{
    int kin; /* keyboard input */
    const int y_all = 21;
    const int x_all = 30;
    curs_set(FALSE);
    g = upecman_init();
    printlab(g);
    mvprintw(y_all, x_all, "<ENTER> para iniciar");
    mvchgat(y_all, x_all, 7, A_BOLD, 4, NULL);
    mvprintw(y_all + 1, x_all, "<q> para voltar ao menu       ");
    mvchgat(y_all + 1, x_all, 4, A_BOLD, 4, NULL);
    refresh();
    g.exit = 0;
    int level = 1;
    while(1)
    {
        do
        {
            kin = getch();
            usleep(16667); /* espere 16666 microseconds para evitar fritar o processador (1/60s) */
        }while(kin != 10 && kin != 'q');

        if(kin == 10)
        {
            g = upecman_init();
            printlab(g);
            g = looping(g,level); /*funcao que contem o laco de jogo */
            mvprintw(y_all, x_all, "<ENTER> para jogar novamente");
            mvchgat(y_all, x_all, 7, A_BOLD, 4, NULL);
            mvprintw(y_all + 1, x_all, "<q> para voltar ao menu       ");
            mvchgat(y_all + 1, x_all, 4, A_BOLD, 4, NULL);
            if(g.exit == 3)
            {
                if(level == 3)
                {
                    mvprintw(13, 30, "VITORIA! Parabens! Voce zerou os 3 niveis!!");
                    mvchgat(13, 30, 50, A_BOLD, 4, NULL);
                    mvprintw(y_all, x_all, "<ENTER> reiniciar o jogo");
                    mvchgat(y_all, x_all, 7, A_BOLD, 4, NULL);
                    level = 1;
                }
                else
                {
                    mvprintw(13, 30, "VITORIA!");
                    mvchgat(13, 30, 10, A_BOLD, 4, NULL);
                    mvprintw(y_all, x_all, "<ENTER> para iniciar o proximo nivel");
                    mvchgat(y_all, x_all, 7, A_BOLD, 4, NULL);
                    mvprintw(y_all + 1, x_all + 23, "(em caso de saida o progresso do jogo nao sera salvo)");
                    level++;
                }
            }
            if(g.exit == 2)
            {
                mvprintw(13, 30, "GAME OVER");
                mvchgat(13,  30, 10, A_BOLD, 1, NULL);
            }
        }
        if(kin == 'q')
            break;
        usleep(16667); /* espere 16666 microseconds para evitar fritar o processador (1/60s) */
    }
    return;
}
void ready(void)
{
    mvprintw(13, 30, "Preparado!");
    mvchgat(13,  30, 11, A_BOLD, 6, NULL);
    refresh();
    usleep(500000);
    mvprintw(13, 30, "Ja!        ");
    mvchgat(13,  30, 4, A_BOLD, 6, NULL);
    refresh();
    usleep(500000);
    mvprintw(13, 30, "          ");
    return;

}
/* ---------------------------------------------------------------------- */
/* looping principal do jogo */

/*obs em relacao a velocidade dos fantasmas: Decidimos trabalhar com o vinel 1; 3 e 5 do jogo original. Nesta versao, a velocidade do pacman esta associada com o 'usleep', ou seja ela esta sempre em 100%, o que vai mudar e o valor do delay. Com isso, a velocidade dos fantasmas sera baseadao delay do pacman. No nivel 1: %90; 2 e 3: aproximadamente %95*/

t_game looping(t_game g, int level)
{
    int pause = 0;
    g.exit = 0; /*g.exit: || 0: desligado || 1: encerramento pelo botao de saida || 2: morte do pacman || 3: vitoria */
    int rep = 1;
    int delay = 249990; /*velocidade do pacman no nivel 1 || 1/60s * 15*/
    int input;
    for(int b = 0; b < 4; b++)/*zerando timer*/
        g.time[b] = 0;
    g.contador = 0;

    g.level = level;
    if(g.level == 2)
        delay *= 0.9; /*pacman 10% mais rapido no nivel 2*/
    if(g.level == 3)
        delay *= 0.8; /*pacman 20% mais rapido no nivel 3*/
    ready();/*aviso de preparado*/
    while(1)
    {
        print_buttom(pause);/*printa botao de pausa e de saida*/
        input = getch(); /*entrada do teclado*/
        if(input == 'q') /*Botao de saida*/
            g.exit = 1;
        if(input == 'p') /*Botao de pausa || 1: ligado || 0: desligado */
        {
            if(pause == 0)
                pause = 1;
            else
                pause = 0;
        }
        if(pause == 0)
        {
            rep++;
            g = call_ghost(g, rep);
            g = timer(g, delay);
            g = main_pacman(g, input);
            g = checar_vitoria(g);
            print_placar(g);
        }
        refresh();
        usleep(delay);
        if(g.exit != 0) /*Condicao para encerramento da partida*/
            break;
    }
    return g;
}

/*---------------------------------------------------------------*/
/*printando botao de pausa e saida*/
void print_buttom(int pause)
{
    mvprintw(21, 30, "<q> para sair       ");
    mvchgat(21, 30, 4, A_BOLD, 1, NULL);
    if(pause == 0)
        mvprintw(22, 30, "<p> para pausar      ");
    else
        mvprintw(22, 30, "<p> para retornar");
    mvchgat(22, 30, 4, A_BOLD, 4, NULL);
    return;
}
/*=======================================================================================
 * TIMER DO JOGO
 */
t_game timer(t_game g, int delay)
{
    g.time[2] += delay / 1000;
    if(g.time[2] >= 1000) //sec
    {
        g.time[1]++;
        g.time[2] %= 1000;
    }
    if(g.time[1] >= 60)
    {
        g.time[0]++;
        g.time[1] %= 60;
    }
    return g;

}
/*====================================================================
 * PLACAR
 */
void print_placar(t_game g)
{
    const int y_all = 15;
    const int x_all = 30;

    switch(g.pacman.life)
    {
        case 3:
            mvprintw(y_all, x_all, "vidas:  @@@        ");
            mvchgat(y_all, x_all + 8, 3, A_BOLD, 6, NULL);
            break;
        case 2:
            mvprintw(y_all, x_all, "vidas:  X@@");
            mvchgat(y_all, x_all + 9, 3, A_BOLD, 6, NULL);
            mvchgat(y_all, x_all + 8, 1, A_BOLD, 1, NULL);
            break;
        case 1:
            mvprintw(y_all, x_all, "vidas:  XX@");
            mvchgat(y_all, x_all + 10, 3, A_BOLD, 6, NULL);
            mvchgat(y_all, x_all + 8, 2, A_BOLD, 1, NULL);
            break;
        case 0:
            mvprintw(y_all, x_all, "vidas:  XXX");
            mvchgat(y_all, x_all + 8, 3, A_BOLD, 1, NULL);
            break;
    }
    mvprintw(y_all + 1, x_all, "nivel:   %i", g.level);
    mvprintw(y_all + 2, x_all, "pontuacao: %d", g.pacman.score);
    mvprintw(y_all + 3, x_all, "tempo: %02i:%02i:%03i   ", g.time[0], g.time[1], g.time[2]);

    return;
}


/*=======================================================================================
 * FANTASMAS */

/* ---------------------------------------------------------------------- */
/*regula a velocidade dos fantasmas de acordo com o level*/
t_game call_ghost(t_game g, int rep)
{
    if(rep % 10 != 0 && g.level == 1)
        g = main_ghost(g, rep);
    if(rep % 20 != 0 &&(g.level == 2 || g.level == 3))
        g = main_ghost(g, rep);

    return g;
}
/* ---------------------------------------------------------------------- */
/*funcao principal dos fantasmas*/
t_game main_ghost(t_game g, int rep)
{
    int f;
    g.contador++;

    for(f = blinky; f <= clyde; f++) /*laco principal dos fantasmas*/
    {

        /*analise de colisao || obs: distribuido em diversas partes do codigo para melhorar a precisao*/
        if(g.ghost[f].pos.y == g.pacman.pos.y && g.ghost[f].pos.x == g.pacman.pos.x)
            g = colisao(g, f);
        /*salvado posicao anterios do fantasma:*/
        g.ghost[f].lc = g.lab[g.ghost[f].pos.y][g.ghost[f].pos.x];
        g.lab[g.ghost[f].pos.y][g.ghost[f].pos.x] = g.ghost[f].lc;
        g.ghost[f].pos.ly = g.ghost[f].pos.y;
        g.ghost[f].pos.lx = g.ghost[f].pos.x;

        /*analise de colisao || obs: distribuido em diversas partes do codigo para melhorar a precisao*/
        /*saida dos fantasmas da casa no inicio do jogo:*/
        if(f != 0 && g.time[1] <= 9)
        {
            if(g.ghost[f].pos.y > 8 && g.ghost[f].pos.y < 12)
                if(g.ghost[f].pos.x > 8 && g.ghost[f].pos.x < 12)
                    g.lab[g.ghost[f].pos.ly][g.ghost[f].pos.lx] = ' ';
            g = porta_ghost(g, f);
        }
        else
            g = ghost_mod(g, f);
        g = mudamodo(g, f);
        g = ghost_target(g, f);

        /*desaceleracao dos fantasmas no portal:*/
        if(g.ghost[f].pos.y == 10 && (g.ghost[f].pos.x < 4 || g.ghost[f].pos.x > 15) )
        {
                if(g.contador % 10 == 0 || g.contador % 3 == 0)
                    g = ghost_pos(g, f);
        }
        else
            g = ghost_pos(g, f);

        /*analise de colisao || obs: distribuido em diversas partes do codigo para melhorar a precisao*/
        if(g.ghost[f].pos.y == g.pacman.pos.y && g.ghost[f].pos.x == g.pacman.pos.x)
            g = colisao(g, f);

        print_ghost(g, f, rep);
    }
    return g;
}

/* ---------------------------------------------------------------------- */
/*desloca as coordenadas dos fantasmas*/
t_game ghost_pos(t_game g, int f)
{

    int x, y; /*coordenadas dos fantasmas*/
    y = g.ghost[f].pos.y;
    x = g.ghost[f].pos.x;

        switch(g.ghost[f].dir)
        {
            case up:
                if(g.lab[y - 1][x] != '#')
                    if(g.lab[y - 1][x] != 'B' && g.lab[y - 1][x] != 'C')
                        if(g.lab[y - 1][x] != 'P' && g.lab[y - 1][x] != 'I')
                            g.ghost[f].pos.y--;
                break;
            case left:
                if(g.lab[y][x - 1] != '#')
                {
                if(g.lab[y][x - 1] != 'B' && g.lab[y][x - 1] != 'C')
                    if(g.lab[y - 1][g.ghost[f].pos.x] != 'P' && g.lab[y][x - 1] != 'I')
                        g.ghost[f].pos.x--;
            }
            break;
        case down:
            if(g.lab[y + 1][x] != '#')
            {
                if(g.lab[y + 1][x] != 'B' && g.lab[y + 1][x] != 'C')
                    if(g.lab[y + 1][x] != 'P' && g.lab[y + 1][x] != 'I')
                        g.ghost[f].pos.y++;
            }
            break;
        case right:
            if(g.lab[y][x + 1] != '#')
            {
                if(g.lab[y][x + 1] != 'B' && g.lab[y][x + 1] != 'C')
                    if(g.lab[y + 1][x] != 'P' && g.lab[y][x + 1] != 'I')
                        g.ghost[f].pos.x++;
            }
            break;
    }
    /*portais:*/
    if(g.ghost[f].pos.y == 10 && g.ghost[f].pos.x < 0)
        g.ghost[f].pos.x = 19;
    if(g.ghost[f].pos.y == 10 && g.ghost[f].pos.x > 19)
        g.ghost[f].pos.x = 0;

    return g;
}

/* ---------------------------------------------------------------------- */
/*ordena de forma crescente as quatro hipotenusas e as armazena em uma matriz*/
double ordena_hip(double matrix[2][4], int c, int n)
{
    int x, y;
    double temp[2][1];
    const int tam = 4;

    for(x = 0; x < tam; x++)
    {
        for(y = x + 1; y < tam; y++)
        {
            if(matrix[1][x] > matrix[1][y])
            {
                temp[0][0] = matrix[0][x];
                temp[1][0] = matrix[1][x];
                matrix[1][x] = matrix[1][y];
                matrix[0][x] = matrix[0][y];
                matrix[0][y] = temp[0][0];
                matrix[1][y] = temp[1][0];
            }
        }
    }
    return matrix[n][c];
}

/* ---------------------------------------------------------------------- */
/*calcula as quatro hipotenusas formadas entre o fantasma e seu target*/
double hipotenusa(t_game g, double hip[4], int f, int c)
{

    int cath_y, cath_x;
    /*hipotenusa caso dir = up*/
    cath_y = g.ghost[f].starget.y - (g.ghost[f].pos.y - 1);
    cath_x = g.ghost[f].starget.x - g.ghost[f].pos.x;
    hip[0] = sqrt(pow(cath_x, 2) + pow(cath_y, 2));
    /*hipotenusa caso dir = left*/
    cath_y = g.ghost[f].starget.y - g.ghost[f].pos.y;
    cath_x = g.ghost[f].starget.x - (g.ghost[f].pos.x - 1);
    hip[1] = sqrt(pow(cath_x, 2) + pow(cath_y, 2));
    /*hipotenusa caso dir = down*/
    cath_y = g.ghost[f].starget.y - (g.ghost[f].pos.y + 1);
    cath_x = g.ghost[f].starget.x - g.ghost[f].pos.x;
    hip[2] = sqrt(pow(cath_x, 2) + pow(cath_y, 2));
    /*hipotenusa caso dir = right*/
    cath_y = g.ghost[f].starget.y - g.ghost[f].pos.y;
    cath_x = g.ghost[f].starget.x - (g.ghost[f].pos.x + 1);
    hip[3] = sqrt(pow(cath_x, 2) + pow(cath_y, 2));

    return hip[c];
}

/* ---------------------------------------------------------------------- */
/*printa os fantasmas na tela*/
void print_ghost(t_game g, int f, int rep)
{
    mvprintw(g.ghost[f].pos.ly, g.ghost[f].pos.lx, "%c", g.ghost[f].lc);
    if(g.ghost[f].lc == '%')
        mvchgat(17, 9, 1, A_BOLD, 1, NULL);

    switch(f)
    {
        case 0:
            mvprintw(g.ghost[f].pos.y, g.ghost[f].pos.x, "B");
            break;
        case 1:
            mvprintw(g.ghost[f].pos.y, g.ghost[f].pos.x, "P");
            break;
        case 2:
            mvprintw(g.ghost[f].pos.y, g.ghost[f].pos.x, "I");
            break;
        case 3:
            mvprintw(g.ghost[f].pos.y, g.ghost[f].pos.x, "C");
            break;
    }
    /*cores normais*/
    if(g.ghost[f].mode < afraid)
        mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, f + 1, NULL);
    else
        /*modo afraid || caracter branco com o fundo azul*/
        if(g.ghost[f].mode == afraid)
        {
            if(g.ghost[f].time - g.time[1] < 4)
            {
                /*"pisca" a cor no modo afraid quando o tempo deste modo esta chegando ao fim*/
                if(rep % 2 == 0)
                    mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, 7, NULL);
                else
                    mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, 12, NULL);
            }
            else
                mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, 7, NULL);
        }
        else
            if(g.ghost[f].mode == dead)
                mvchgat(g.ghost[f].pos.y, g.ghost[f].pos.x, 1, A_BOLD, 5, NULL);
}
/* ---------------------------------------------------------------------- */
/*saida dos fantasmas da casa no inicio do jogo*/
t_game porta_ghost(t_game g, int f)
{
    switch(f)
    {
        case 1:

            if(g.time[1] > 2 && g.time[1] <= 4)
            {
                g.ghost[1].starget.y = 7;
                g.ghost[1].starget.x = 9;
            }
            else
            {
                g = mudamodo(g, f);
                g = ghost_mod(g, f);
            }
            break;
        case 2:

            if(g.time[1] > 3 && g.time[1] <= 6)
            {
                g.ghost[2].starget.x = 7;
                g.ghost[2].starget.y = 7;
            }
            else
            {
                g = mudamodo(g, f);
                g = ghost_mod(g, f);
            }
            break;

        case 3:
            if(g.time[1] > 7)
            {
                g.ghost[3].starget.y = 7;
                g.ghost[3].starget.x = 9;
            }
            else
            {
                g = mudamodo(g, f);
                g = ghost_mod(g, f);
            }
            break;
    }
    return g;
}

/* ---------------------------------------------------------------------- */
/*colisao entre pacman e fantasma*/
t_game colisao(t_game g, int f)
{
    if(g.ghost[f].mode == afraid)
    {
        g.pacman.score += 750;
        g.ghost[f].mode = dead;
    }
    else
        if(g.ghost[f].mode < afraid)
            g = dead_pacman(g);
    if(g.pacman.life == 0)
        g.exit = 2;
    return g;
}

/* ---------------------------------------------------------------------- */
/*troca o modo dos fantasmas*/
t_game mudamodo(t_game g, int f)
{

    if(g.ghost[f].mode == dead)
    {
        if(g.ghost[f].pos.y == g.ghost[f].starget.y)
            if(g.ghost[f].pos.x == g.ghost[f].starget.x)
                g.ghost[f].mode = scatter;
    }
    if(g.time[1] >= g.ghost[f].time && g.ghost[f].mode == afraid)
    {
        g.ghost[f].mode = scatter;
        g.ghost[f].time = 0;
    }
    switch(g.level)
    {
        case 1:
            g = level1_mode(g, f);
            break;
        case 2:
            g = level2_mode(g, f);
            break;
        case 3:
            g = level3_mode(g, f);
            break;
    }
    return g;
}
/* ---------------------------------------------------------------------- */
/*define o target dos fantasmas em seus respectivos modos*/
t_game ghost_mod(t_game g, int f)
{

    switch(g.ghost[f].mode)
    {
        case chase:
            g = mode_chase(g);
            if(g.time[1] > 9)
                g = casa_ghost(g, f);
            break;
            case scatter:
            g = mode_scatter(g);
            if(g.time[1] > 9)
                g = casa_ghost(g, f);
            break;
            case dead:
            g = mode_dead(g, f);
            break;
            default:
            /*os targets dos fantasmas no modo afraid estao na funcao "t_game pastilhao(t_game g)"*/
            break;
    }
    return g;
}
/* ---------------------------------------------------------------------- */
/*targets no mode chase*/
t_game mode_chase(t_game g)
{
    /*blinky*/
    g.ghost[0].starget.y = g.pacman.pos.y;
    g.ghost[0].starget.x = g.pacman.pos.x;
    g = chase_pinky(g);/*pinky*/
    g = chase_inky(g);/*inky*/
    g = chase_clyde(g);/*clyde*/

    return g;
}
/* ---------------------------------------------------------------------- */
/*targets no mode dead*/
t_game mode_dead(t_game g, int f)
{
    g.ghost[0].starget.y = 9;
    g.ghost[0].starget.x = 9;
    /*pinky*/
    g.ghost[1].starget.y = 10;
    g.ghost[1].starget.x = 9;
    /*inky*/
    g.ghost[2].starget.y = 10;
    g.ghost[2].starget.x = 10;
    /*clyde*/
    g.ghost[3].starget.y = 10;
    g.ghost[3].starget.x = 11;
    if(g.ghost[f].pos.y == g.ghost[f].starget.y && g.ghost[f].pos.x == g.ghost[f].starget.x)
        g.ghost[f].mode = scatter;


    return g;
}
/* ---------------------------------------------------------------------- */
/*target dos fantasmas no modo scatter*/
t_game mode_scatter(t_game g)
{
    /*blinky*/
    g.ghost[0].starget.y = 0;
    g.ghost[0].starget.x = 19;
    /*pinky*/
    g.ghost[1].starget.y = 0;
    g.ghost[1].starget.x = 0;
    /*inky*/
    g.ghost[2].starget.y = 22;
    g.ghost[2].starget.x = 19;
    /*clyde*/
    g.ghost[3].starget.y = 22;
    g.ghost[3].starget.x = 0;

    return g;
}
t_game chase_inky(t_game g)
{
    int py, px, vetor_y, vetor_x, p_escalar, proj_x, proj_y;
    float mag;
    if(g.pacman.dir == up)
    {
        py = g.pacman.pos.y - 2;
        px = g.pacman.pos.x - 2;
    }
    else
    {
        py = g.pacman.pos.y - 2;
        px = g.pacman.pos.x;
    }
    vetor_y = py - g.ghost[0].pos.y;
    vetor_x = px - g.ghost[0].pos.x;
    p_escalar = px * vetor_x + py * vetor_y;/*produto escalar*/
    mag = sqrt(pow(px, 2) + pow(py, 2)); /*magnitude*/
    proj_x = round(px * p_escalar/pow(mag, 2));/*projecao do vetor x*/
    proj_y = round(py * p_escalar/pow(mag, 2));/*projecao do vetor y*/
    g.ghost[inky].starget.x = proj_x + px;
    g.ghost[inky].starget.y = proj_y + py;

    return g;
}
/* ---------------------------------------------------------------------- */
/*target do Clyde no modo chase*/
t_game chase_clyde(t_game g)
{
    int cath_x, cath_y, dist;
    cath_y = g.pacman.pos.y - g.ghost[3].pos.y;
    cath_x = g.pacman.pos.x - g.ghost[3].pos.x;
    dist = sqrt(pow(cath_x, 2) + pow(cath_y, 2));
    if(dist <= 8)
    {
        g.ghost[3].starget.y = 22;
        g.ghost[3].starget.x = 0;
    }
    else
    {
        g.ghost[3].starget.y = g.pacman.pos.y;
        g.ghost[3].starget.x = g.pacman.pos.x;
    }
    return g;
}
/* ---------------------------------------------------------------------- */
/*target do Pinky no modo chase*/
t_game chase_pinky(t_game g)
{
    switch(g.pacman.dir)
    {
        case up:
            g.ghost[1].starget.y = g.pacman.pos.y - 4;
            g.ghost[1].starget.x = g.pacman.pos.x;
            break;
        case right:
            g.ghost[1].starget.y = g.pacman.pos.y;
            g.ghost[1].starget.x = g.pacman.pos.x + 4;
            break;
        case left:
            g.ghost[1].starget.y = g.pacman.pos.y;
            g.ghost[1].starget.x = g.pacman.pos.x - 4;
            break;
        case down:
            g.ghost[1].starget.y = g.pacman.pos.y + 4;
            g.ghost[1].starget.x = g.pacman.pos.x;
            break;
    }
    return g;
}
/* ---------------------------------------------------------------------- */
/*saida dos fantasmas de sua casa apos a sua morte*/
t_game casa_ghost(t_game g, int f)
{
    if(g.ghost[f].pos.y > 7 && g.ghost[f].pos.y < 13 && (g.ghost[f].pos.x > 7 && g.ghost[f].pos.x < 12))
    {
        if(g.ghost[f].mode < dead)
        {
            g.ghost[f].starget.y = 7;
            g.ghost[f].starget.x = 9;
            if(g.ghost[f].pos.y == 7 && g.ghost[f].pos.x == 9)
            {
                g = mudamodo(g, f);
                g = ghost_mod(g, f);
            }
        }
    }
    return g;
}

/* ---------------------------------------------------------------------- */
/*testa as rodas dos fantasmas em direcao ao target em ordem crescente*/
t_game ghost_target(t_game g, int f)
{
    double matrix[2][4];
    double hip[4];
    int c, n;
    const int tam = 4;

    for(c = 0; c < tam; c++)
        matrix[0][c] = c;
    for(c = 0; c < tam; c++)
    {
        hip[c] = hipotenusa(g, hip, f, c); /* funcao que calcula as hipotenusas */
        matrix[1][c] = hip[c];
    }

    for(n = 0; n < 2; n++)
    {
        for(c = 0; c < tam; c++)
            matrix[n][c] = ordena_hip(matrix, c, n);
    }
    g = ghost_dir(g, f, matrix);

    return g;
}
/* ---------------------------------------------------------------------- */
/*direciona o fantasma*/
t_game ghost_dir(t_game g, int f, double matrix[2][4])
{
    int c, tam = 4;
    /*variaveis usadas para armazenar as coodenadas da safe zone*/
    int x = g.ghost[f].pos.x;
    int y = g.ghost[f].pos.y;
    for(c = 0; c < tam; c++)
    {
        if(matrix[0][c] == 0)/*testa se a melhor rora e a de cima*/
        {
            if(g.lab[g.ghost[f].pos.y - 1][g.ghost[f].pos.x] != '#')
            {
                    if(g.ghost[f].lastdir != down)
                    {
                        if(y == 16 || y == 6 || y == 7 || y == 17 )/*safe zone*/
                        {
                            if(x < 6 || x > 12)
                            {
                                g.ghost[f].dir = up;
                                break;
                            }
                        }
                        else
                        {
                            g.ghost[f].dir = up;
                            break;
                        }
                    }
            }
        }
        else
            if(matrix[0][c] == 1)/*testa se a melhor rota e a da esquerda*/
            {
                if(g.lab[g.ghost[f].pos.y][g.ghost[f].pos.x - 1] != '#')
                {
                    if(g.ghost[f].lastdir != right)
                    {
                        g.ghost[f].dir = left;
                        break;
                    }
                }
            }
            else
                if(matrix[0][c] == 2)/*testa se a melhor rota e a de baixo*/
                {
                    if(g.ghost[f].mode == dead)
                    {
                        if(g.lab[g.ghost[f].pos.y + 1][g.ghost[f].pos.x] != '#')
                            if(g.ghost[f].lastdir != up)
                            {
                                g.ghost[f].dir = down;
                                break;
                            }
                    }
                    else
                        if(g.lab[g.ghost[f].pos.y + 1][g.ghost[f].pos.x] != '#' && (g.lab[g.ghost[f].pos.y + 1][g.ghost[f].pos.x] != '-'))
                        {
                            if(g.ghost[f].lastdir != up)
                            {
                                g.ghost[f].dir = down;
                                break;
                            }
                        }
                }
                else
                    if(matrix[0][c] == 3)/*testa se a melhor rota e a da direita*/
                    {
                        if(g.lab[g.ghost[f].pos.y][g.ghost[f].pos.x + 1] != '#')
                        {
                            if(g.ghost[f].lastdir != left)
                            {
                                g.ghost[f].dir = right;
                                break;
                            }
                        }
                    }
    }
    g.ghost[f].lastdir = g.ghost[f].dir;
    return g;
}
/*--------------------------------------------------------
 * COMPORTAMENTO DOS FANTASMAS DE ACORDO COM O LEVEL */

/*------------------------------ */
/*level 1*/
t_game level1_mode(t_game g, int f)
{
    if( g.ghost[f].mode < afraid)
    {
        if(g.time[1] < 7)
            g.ghost[f].mode = scatter;
        else
            if(g.time[1] < 27)
                g.ghost[f].mode = chase;
            else
                if(g.time[1] < 34)
                    g.ghost[f].mode = scatter;
                else
                    if(g.time[1] < 54)
                        g.ghost[f].mode = chase;
                    else
                        if(g.time[1] < 59)
                            g.ghost[f].mode = scatter;
                        else
                            if(g.time[1] < 79)
                                g.ghost[f].mode = chase;
                            else
                                if(g.time[1] < 84)
                                    g.ghost[f].mode = scatter;
    }
    return g;
}

/*------------------------------ */
/*level 2*/
t_game level2_mode(t_game g, int f)
{
    if( g.ghost[f].mode < afraid)
    {
        if(g.time[1] < 7)
            g.ghost[f].mode = scatter;
        else
            if(g.time[1] < 27)
                g.ghost[f].mode = chase;
            else
                if(g.time[1] < 34)
                    g.ghost[f].mode = scatter;
                else
                    if(g.time[1] < 54)
                        g.ghost[f].mode = chase;
                    else
                        if(g.time[1] < 59)
                            g.ghost[f].mode = scatter;
                        else
                            if(g.time[0] < 18)
                                g.ghost[f].mode = chase;
                            else
                                if(g.time[1] < 5)
                                    g.ghost[f].mode = scatter;
    }
    return g;
}

/*------------------------------ */
/*level 3*/
t_game level3_mode(t_game g, int f)
{
    if( g.ghost[f].mode < afraid)
    {
        if(g.time[1] < 5)
            g.ghost[f].mode = scatter;
        else
            if(g.time[1] < 25)
                g.ghost[f].mode = chase;
            else
                if(g.time[1] < 30)
                    g.ghost[f].mode = scatter;
                else
                    if(g.time[1] < 50)
                        g.ghost[f].mode = chase;
                    else
                        if(g.time[0] < 55)
                            g.ghost[f].mode = scatter;
                        else
                            if(g.time[0] < 18 && g.time[1] < 13)
                                g.ghost[f].mode = chase;
                            else
                                if(g.time[1] < 1)
                                    g.ghost[f].mode = scatter;
    }

    return g;
}

/*==================================================================
 * PACMAN */

/* ---------------------------------------------------------------------- */
/*reune todas as funcoes envolvendo o Pacman*/
t_game main_pacman(t_game g, int input)
{
    g = dir_pacman(g, input);
    g = move_pacman(g);
    g = score_pacman(g);
    print_pacman(g);
    /*analise de colisao entre o pacman e os fantasmas || obs: distribuido em diversas partes do codigo para melhorar a precisao*/
    for(int f = 0; f < 4; f++)
        if(g.ghost[f].pos.y == g.pacman.pos.y && g.ghost[f].pos.x == g.pacman.pos.x)
            g = colisao(g, f);
    return g;
}
/* ---------------------------------------------------------------------- */
/*recebe o dir do pacman e muda suas coordenadas*/
t_game move_pacman(t_game g)
{
    g.pacman.pos.lastx = g.pacman.pos.x;
    g.pacman.pos.lasty = g.pacman.pos.y;
    g.lab[g.pacman.pos.y][g.pacman.pos.x] = ' ';

    if(g.pacman.dir == up && g.lab[g.pacman.pos.y - 1][g.pacman.pos.x] != '#')
        g.pacman.pos.y--;
    else
        if(g.pacman.dir == down && g.lab[g.pacman.pos.y + 1][g.pacman.pos.x] != '#' && g.lab[g.pacman.pos.y + 1][g.pacman.pos.x] != '-')
            g.pacman.pos.y++;
        else
            if(g.pacman.dir == right && g.lab[g.pacman.pos.y][g.pacman.pos.x + 1] != '#' && g.lab[g.pacman.pos.y][g.pacman.pos.x + 1] != '-')
                g.pacman.pos.x++;
            else
                if(g.pacman.dir == left && g.lab[g.pacman.pos.y][g.pacman.pos.x - 1] != '#' && g.lab[g.pacman.pos.y][g.pacman.pos.x - 1] != '-')
                    g.pacman.pos.x--;

    if(g.pacman.pos.y == 10 && g.pacman.pos.x < 0) /*portal da esquerda*/
        g.pacman.pos.x = 19;
    if(g.pacman.pos.y == 10 && g.pacman.pos.x > 19) /*portal da direita*/
        g.pacman.pos.x = 0;
    return g;
}
/* ---------------------------------------------------------------------- */
/*recebe a entrada do teclado e determina a direcao do Pacman*/
t_game dir_pacman(t_game g, int input)
{
    /*premove:*/
    if(input == -1)
    {
        input = g.pm;
        g.pm = -1;
    }
    switch(input)
    {
        case KEY_UP:
            if(g.lab[g.pacman.pos.y - 1][g.pacman.pos.x] != '#')
            {
                g.pacman.dir = up;
                g.pm = -1;
            }
            else
                g.pm = KEY_UP;
            break;

        case KEY_DOWN:
            if(g.lab[g.pacman.pos.y + 1][g.pacman.pos.x] != '#' && g.lab[g.pacman.pos.y + 1][g.pacman.pos.x] != '-')
            {
                g.pacman.dir = down;
                g.pm = -1;
            }
            else
                g.pm = KEY_DOWN;
            break;

        case KEY_RIGHT:
            if(g.lab[g.pacman.pos.y][g.pacman.pos.x + 1] != '#')
            {
                g.pacman.dir = right;
                g.pm = -1;
            }
            else
                g.pm = KEY_RIGHT;
            break;

        case KEY_LEFT:
            if(g.lab[g.pacman.pos.y][g.pacman.pos.x - 1] != '#')
            {
                g.pacman.dir = left;
                g.pm = -1;
            }
            else
                g.pm = KEY_LEFT;
            break;
    }

    return g;
}

/* ---------------------------------------------------------------------- */
/*consequencia de quando o Pacman come o pastilhao*/
t_game pill(t_game g)
{
    int f, temp;
    srand(time(NULL));
    for(f = blinky; f <= clyde; f++)
    {
        if(g.ghost[f].lastdir == up)
            g.ghost[f].dir = down;
        else
            if(g.ghost[f].lastdir == left)
                g.ghost[f].dir = right;
            else
                if(g.ghost[f].lastdir == down)
                    g.ghost[f].dir = up;
                else
                    g.ghost[f].lastdir = left;

        g.ghost[f].mode = afraid;
        if(g.time[1] > 47)
        {
            temp = 60 - g.time[1];
            g.ghost[f].time -= temp;
            g.ghost[f].time += 12;
        }
        else
            g.ghost[f].time = g.time[1] + 12;
        g.ghost[f].starget.x = rand() % 20;
        g.ghost[f].starget.y = rand() % 23;
    }
    return g;
}

/* ---------------------------------------------------------------------- */
/*Printa o pacman na tela*/
void print_pacman(t_game g)
{
    mvprintw(g.pacman.pos.lasty, g.pacman.pos.lastx, " ");
    mvprintw(g.pacman.pos.y, g.pacman.pos.x, "@");
    mvchgat(g.pacman.pos.y, g.pacman.pos.x, 1, A_BOLD, 6, NULL);
    refresh();
}
/* ---------------------------------------------------------------------- */
/*pontuacao e consequencias para quando o pacman come o pastilhao, cereja e as pelotas*/
t_game score_pacman(t_game g)
{
    if(g.lab[g.pacman.pos.y][g.pacman.pos.x] == '.')
    {
        g.pacman.score += 10;
        g.pacman.pacdots++;
    }
    if(g.lab[g.pacman.pos.y][g.pacman.pos.x] == 'o')
    {
        g = pill(g);
        g.pacman.score += 50;
        g.pacman.pill++;
    }
    if(g.lab[g.pacman.pos.y][g.pacman.pos.x] == '%')
        g.pacman.score += 500;
    if(g.pacman.pacdots >= 70 && g.pacman.cherry == 0)
        g = cherry(g);
    if(g.time[1] >= g.pacman.tempocereja)
    {
        g.lab[17][9] = ' ';
        mvprintw(17, 9, "%c", g.lab[17][9]);
    }
    return g;
}
/* ---------------------------------------------------------------------- */
/*cereja*/
t_game cherry(t_game g)
{
    int temp;
    g.pacman.cherry = 1;
    g.lab[17][9] = '%';
    mvprintw(17, 9, "%c", g.lab[17][9]);
    mvchgat(17, 9, 1, A_BOLD, 1, NULL);
    mvprintw(LABL , 18, "  ");
    if(g.time[1] > 49)
    {
        temp = 60 - g.time[1];
        g.pacman.tempocereja = 10 - temp;
    }
    else
        g.pacman.tempocereja = g.time[1] + 10;
    return g;
}
/* ---------------------------------------------------------------------- */
/*checa se co Pacman comeu todas as pelotas e pastilhoes*/
t_game checar_vitoria(t_game g)
{
    if(g.pacman.pill == 4 && g.pacman.pacdots == 184)
        g.exit = 3;

    return g;
}
/* ---------------------------------------------------------------------- */
/*reposiciona os personagens quando o pacman perde uma vida*/
t_game dead_pacman(t_game g)
{
    int f;
    for(f = blinky; f <= clyde; f++)
    {
        g.ghost[f].mode = 1;
        mvprintw(g.ghost[f].pos.y, g.ghost[f].pos.x, "%c", g.ghost[f].lc);
    }
    mvprintw(g.pacman.pos.y, g.pacman.pos.x, " ");
    g.pacman.pos.y = 17;
    g.pacman.pos.x = 9;
    g.pacman.dir = left;
    g.ghost[0].pos.y = 7;
    g.ghost[0].pos.x = 9;
    g.ghost[1].pos.y = 9;
    g.ghost[1].pos.x = 10;
    g.ghost[2].pos.y = 10;
    g.ghost[2].pos.x = 10;
    g.ghost[3].pos.y = 11;
    g.ghost[3].pos.x = 10;
    g.pacman.life--;
    if(g.pacman.life != 0)
        ready();/*aviso de preparado*/
    for(int c = 0; c < 3; c++)
        g.time[c] = 0;

    return g;
}
