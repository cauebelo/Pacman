/* definitions */

#ifndef VERSION
#define VERSION "20160529.013231" /**< Version Number */
#endif

/* Debug */
#ifndef DEBUG /* gcc -DDEBUG=1 */
#define DEBUG 0 /**< Activate/deactivate debug mode */
#endif

/** @brief Debug message if DEBUG on */
#define IFDEBUG(M) if(DEBUG) fprintf(stderr, "[DEBUG file:%s line:%d]: " M "\n", __FILE__, __LINE__); else {;}

/* limits */
#define SBUFF 256 /**< String buffer */
#define LABL 23 /**< Number of lines in labyrinth */
#define LABC 21 /**< Number of columns in labyrinth */
#define DY 21 /*linhas do titulo*/
#define DX 90 /*colunas do titulo*/
#define DSY 9 /*colunas da desenho na pagina do jogo*/
#define DSX 15/*linhas da desenho na pagina do jogo*/

/* ---------------------------------------------------------------------- */
/* globals */

static int verb = 0; /**< verbose level, global within the file */

const char labmodel[LABL][LABC] =
{
    /*01234567890123456789*/
    {"####################"},    /* 0*/
    {"#........##........#"},    /* 1*/
    {"#o##.###.##.###.##o#"},    /* 2*/
    {"#..................#"},    /* 3*/
    {"#.##.#.######.#.##.#"},    /* 4*/
    {"#....#...##...#....#"},    /* 5*/
    {"####.###.##.###.####"},    /* 6*/
    {"   #.#...B....#.#   "},    /* 7*/
    {"   #.#.##--##.#.#   "},    /* 8*/
    {"####.#.#  P #.#.####"},    /* 9*/
    {"    ...#  I #...    "},    /*10*/
    {"####.#.#  C #.#.####"},    /*11*/
    {"   #.#.######.#.#   "},    /*12*/
    {"   #.#........#.#   "},    /*13*/
    {"####.#.######.#.####"},    /*14*/
    {"#........##........#"},    /*15*/
    {"#o##.###.##.###.##o#"},    /*16*/
    {"#..#.....@......#..#"},    /*17*/
    {"##.#.#.######.#.#.##"},    /*18*/
    {"#....#...##...#....#"},    /*19*/
    {"#.######.##.######.#"},    /*20*/
    {"#..................#"},    /*21*/
    {"####################"}     /*22*/
};
const char g1[DSY][DSX] =
{
    {" pppppppp  "},
    {"ppwwppwwpp "},
    {"ppwbppwbpp "},
    {"pppppppppp "},
    {"pppppppppp "},
    {"p  p  p  p "},
};
const char g2[DSY][DSX] =
{
    {" pppppppp  "},
    {"pp0pppp0pp "},
    {"pppppppppp "},
    {"p/\\/\\/\\/\\p "},
    {"pppppppppp "},
    {"p  p  p  p "},
};
const char pac[DSY][DSX] =
{
    {"   ppppppp "},
    {" ppppppp   "},
    {"pppppp     "},
    {"pppppppp   "},
    {" pppppppppp"},
    {"   pppppp  "},
};

typedef enum e_ghostmode {chase, scatter, afraid, dead} t_ghostmode;
typedef enum e_direction {up, left, down, right} t_direction;
typedef enum e_ghosts {blinky, pinky, inky, clyde} t_ghosts;

typedef struct st_position
{
    int y; /* line */
    int x; /* column */
    int lasty, lastx; /*posicao anterior do pacman*/
    int ly; /*coordenada y anterior*/
    int lx; /*coordenada x anterior*/

} t_pos;

typedef struct st_pacman
{
    t_pos pos; /* current pacman position */
    t_direction dir; /* direction */
    int life; /* how many lifes pacman has */
    int score; /* dot = 10 point; pill = 50 points; ghost = 750 points; cherry = 500 */
    int pacdots;
    int tempocereja;
    int cherry;
    int pill;
} t_pacman;

typedef struct st_ghost
{
    t_pos pos; /* ghost position */
    t_direction dir; /* ghost current direction */
    t_pos starget; /* ghost scatter preferred corner */
    t_ghostmode mode; /* chase, scatter, afraid or dead */
    int lastdir;
    char lc; /*ultimo caracter*/
    int time;
} t_ghost;

typedef struct st_pacdata
{
    char lab[LABL][LABC]; /* the labyrinth map */
    t_pacman pacman; /* pacman data */
    t_ghost ghost[4]; /* ghost[blinky], ghost[pinky], ghost[inky], ghost[clyde] */
    int pm;/*premove*/
    int time[3]; /*time[0] = minutos || time[1] = segundos || time[2] = milisegundos*/
    int level;
    int exit;/*controle de encerramento do looping*/
    int contador;
} t_game;


const char title[DY][DX] =
{
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@...@@@..@@@.......@........@@@,,,,,,,,,,,,,,,,,,,,,xxx@@...@@@@@..@@.........@@..@@@...@"},
    {"@...@@@..@@@..@@@..@...@@@@@@@,,,,,,,,,,,,,,,xxxxxxxxxx@@....@@@...@@...@@@...@@...@@...@"},
    {"@...@@@..@@@.......@...@@@@@@@,,,,,,,,,,,xxxx@@@@@@@@@@@@...@...@..@@...@@@...@@..@.....@"},
    {"@...@@@..@@@..@@@@@@........@@,,,,,,,,,x@@@@@@@@@@@@@@@@@...@@@@@..@@.........@@..@@@...@"},
    {"@...@@@..@@@..@@@@@@...@@@@@@@,,,,,,,,,,,xxxx@@@@@@@@@@@@...@@@@@..@@...@@@...@@..@@@...@"},
    {"@........@@@..@@@@@@........@@,,,,,,,,,,,,,,,xxxxxxxxxx@@...@@@@@..@@...@@@...@@..@@@...@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,,,,,xxx@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@,,,,,,,,,@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"},
    {"                                    ~ GRUPO INKY ~                                       "},

}; /*Titulo interativo do menu do jogo*/

/* ---------------------------------------------------------------------- */
/* prototypes */

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
t_game upecman_init(void); /* initialize game variables */
void printlab(t_game g); /* print the labyrinth */
void art(void); /*arte da tela de jogo*/
/*-----------------------*/
/* Menu e pre jogo: ----*/
void menu(t_game g);
void logo(int cont);
void regras();
void pre_game(t_game g);
void ready(void);

/*-----------------------*/
/* Looping e relacionados:  ----*/
t_game looping(t_game g, int level); //looping principal do jogo
t_game timer(t_game g, int delay);
t_game colisao(t_game g, int f);
t_game checar_vitoria(t_game);
void print_buttom(int pause);
t_game dead_pacman(t_game g);
/*-----------------------*/
/* Pacman: ----*/
t_game main_pacman(t_game g, int input);
t_game move_pacman(t_game g);
t_game dir_pacman(t_game g, int input);
t_game pill(t_game g);
t_game score_pacman(t_game g);
t_game cherry(t_game g);
void print_pacman(t_game g);

/*-----------------------*/
/* Fantasmas: ----*/
t_game call_ghost(t_game g, int rep);
t_game main_ghost(t_game g, int rep);
t_game ghost_target(t_game g, int f);
t_game ghost_pos(t_game g, int f);
t_game ghost_dir(t_game g, int f, double matrix[2][4]);
t_game porta_ghost(t_game g, int f);
t_game mudamodo(t_game g, int f);
t_game level1_mode(t_game g, int f);
t_game level2_mode(t_game g, int f);
t_game level3_mode(t_game g, int f);
double ordena_hip(double matrix[2][4], int c, int n);
double hipotenusa(t_game g, double hip[4], int f, int c);
t_game ghost_mod(t_game g, int f);
t_game casa_ghost(t_game g, int f);
void print_ghost(t_game g, int f, int rep);
t_game chase_inky(t_game g);
t_game chase_clyde(t_game g);
t_game chase_pinky(t_game g);
t_game mode_dead(t_game g, int f);
t_game mode_scatter(t_game g);
t_game mode_chase(t_game g);
/*-----------------------*/
/* Placar: ----*/
void print_placar(t_game g);