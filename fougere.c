#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define taillex  10000
#define tailley  5000
#define scale 1500//echelle de l'image
#define pi M_PI
struct color{//importation structure couleur
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct picture{
    int width; //largeur
    int height; //hauteur
    struct color *pixel;
};
struct vector{
    double x1;
    double y1;
    double x2;
    double y2;
    struct vector* next;
};

struct point{
    double x;
    double y;
};


struct picture new_pic(double largeur, double hauteur){//création d'une image vierge
    struct picture image;
    image.width = largeur;
    image.height = hauteur;
    double taille = largeur*hauteur;
    image.pixel = malloc(taille*sizeof(struct color));//on alloue la mémoire
    for(int i = 0; i< taille; i++){//boucle qui initialise chaque pixel
        image.pixel[i].blue = 0;
        image.pixel[i].green = 0;
        image.pixel[i].red = 0;
    }
    return image;
}

void save_pic(struct picture image, char * fichier){//sauvegarde

    FILE * im = fopen(fichier, "w");
    if(im == NULL){
        printf("erreur de création fichier\n");
    }
    else{
        fprintf(im, "P6");
        fprintf(im, " ");
        fprintf(im, "%d", image.width);//on converti en const char*
        fprintf(im, " ");
        fprintf(im, "%d", image.height);//on converti en const char*
        fprintf(im, " ");
        fprintf(im, "255");
        fprintf(im, " ");
        printf("%s en fichier crée\n",fichier);
    }
    for(double i = 0; i<image.height; i++){
        for(double j = 0; j<image.width; j++){
            int position = i*image.width + j;
            fputc(image.pixel[position].red, im);
            fputc(image.pixel[position].green, im);
            fputc(image.pixel[position].blue, im);
        }
    }

}

void set_pixel(struct picture image, double x, double y, struct color color_c){//fonction qui modifie pixel
    int position = y*(image.width) + x;
    image.pixel[position].red = color_c.red;
    image.pixel[position].green = color_c.green;
    image.pixel[position].blue = color_c.blue;
}

void draw_line(struct picture image, double x1, double y1, double  x2, double y2, struct color color_c){/*il faut
une nouvelle fonction qui trace toutes les droites peu importe le coefficients directeur*/
    double nb_pixels = fmax(fabs(x1-x2),fabs(y1-y2)) + 1;
    if((x1 == x2) && (y1 < y2)){
        for(int i = 0; i<nb_pixels; i++){
            set_pixel(image, x2, y2-i,color_c);
        }
        return;
    }
    else if((x1 == x2) && (y1 > y2)){
        for(int i = 0; i<nb_pixels; i++){
            set_pixel(image, x1, y1 - i,color_c);
        }
        return;
    }
    else if((y1 == y2) && (x1<x2)){
        for(int i = 0; i<nb_pixels; i++){
            set_pixel(image, x1 + i, y1, color_c);
        }
        return;
    }
    else if((x1 > x2) && (y1 == y2)){
        for(int i = 0; i<nb_pixels; i++){
            set_pixel(image, x2 + i, y1,color_c);
        }
        return;
    }
    else{
        for(int i = 0; i<nb_pixels; i++){
            float t = (float) i/ (float) nb_pixels;
            double x = round(x1+ t*(x2 - x1));
            double y = round(y1 + t*(y2 - y1));
            set_pixel(image, x, y, color_c);
        }
        return;
    }
}


// tracé d'une feuille
struct point coordonnes(struct point p1, double angle/*radian*/, double echelle, double distance){//calcul des coordonées d'un point pour une distance d'un point connu à ce point
    while (angle < 0.0) {
            angle += 2 * M_PI;
            }
    while (angle >= (2 * M_PI)) {
            angle -= 2 * M_PI;
        }
    struct point p2 ={0,0};
    p2.x = abs(distance*echelle*cos(angle) + p1.x);
    p2.y = abs(-distance*echelle*sin(angle) + p1.y);
    return p2;
}


void iteratif_devant(struct picture image, struct color color_c,int n){
    //initialisation
    struct point Og = {0,0};
    //point O de départ
    struct point O = {10,424};
    //initialisation point O_G
    struct point O_ginit = {240,424};
    //première droite
    draw_line(image,O.x,O.y,O_ginit.x,O_ginit.y,color_c);
    int a = 1; //compteur itérations
    while((a != n)){
        //Og.x = 0.16*pow(0.86,a)*scale*cos(-0.05*a) + O_ginit.x;
        //Og.y = -0.16*pow(0.86,a)*scale*sin(-0.05*a) + O_ginit.y;
        Og = coordonnes(O_ginit,-0.05*a,scale,pow(0.86,a));
        draw_line(image,Og.x,Og.y,O_ginit.x,O_ginit.y,color_c);
        //on copie les coordonées
        O_ginit.x = Og.x;
        O_ginit.y = Og.y;
        a++;
    }
}
/*
//tracé tige principale récursif
void recursivite_devant1(struct picture fougere, struct point depart, double angle, double echelle, int n,struct color color_c){
    n++;
    if (n>10000){
        return;
    }
    struct point_next;
    struct point next=coordonnes(depart, angle, echelle, 0.16);
    angle=angle-0.05;
    echelle=echelle*0.86;
    draw_line(fougere,depart.x,depart.y,next.x,next.y,color_c);
    recursivite_devant(fougere, next, angle, echelle, n, color_c);
}
void recursivite_devant(struct picture fougere, struct point depart, double angle, double echelle, int n, struct color color_c){
   n++;
   if (n>100){
    return;
   }
   struct point next = coordonnes(depart, angle, echelle, 0.27);
   angle = angle + pi/2;
   echelle= echelle*0.27;
   draw_line(fougere,depart.x,depart.y,next.x,next.y,color_c);
   recursivite_haut(fougere, next, angle, echelle, n,color_c);
}
*/
void recursivite_devant(struct picture fougere, struct point depart, double angle, double echelle,struct color color_c, int n,int power){
    n++;
    if (n>50){
        return;
    }
    struct point next=coordonnes(depart, angle, echelle, 0.16);
    double angle1=angle+0.05*pow(-1,power);
    double echelle1=echelle*0.86;
    draw_line(fougere,depart.x,depart.y,next.x,next.y,color_c);
    recursivite_devant(fougere, next, angle1, echelle1,color_c,n,power);
    struct point next1=coordonnes(depart, angle, echelle, 0.14);
    double echelle2=echelle*0.27;
    double angle2=angle + pi/2;
    recursivite_devant(fougere, next1, angle2, echelle2,color_c, n,1);
    struct point next2=coordonnes(depart, angle, echelle, 0.1);
    double echelle3=echelle*0.30;
    double angle3=angle - pi/2;
    recursivite_devant(fougere, next2, angle3, echelle3,color_c, n,0);
}

void recursivite_devantopti(struct picture fougere, struct point depart, double angle, double echelle,struct color color_c, int n,int power){
    int a = rand()%30;
    color_c.green = 255-a;
    color_c.red = 255-a*3+20;
    n++;
    if (n>40){
        return;
    }
    struct point next=coordonnes(depart, angle, echelle, 0.16);
    double angle1=angle+0.05*pow(-1,power);
    double echelle1=echelle*0.86;
    draw_line(fougere,depart.x,depart.y,next.x,next.y,color_c);
    recursivite_devantopti(fougere, next, angle1, echelle1,color_c,n,power);
    struct point next1=coordonnes(depart, angle, echelle, 0.14);
    double echelle2=echelle*0.27;
    double angle2=angle + pi/2;
    recursivite_devantopti(fougere, next1, angle2, echelle2,color_c, n+5,1);
    struct point next2=coordonnes(depart, angle, echelle, 0.1);
    double echelle3=echelle*0.30;
    double angle3=angle - pi/2;
    recursivite_devantopti(fougere, next2, angle3, echelle3,color_c, n+5,0);
}

int main(){
    clock_t start, end;
    double cpu_time_used;
    // Obtenir le temps de départ
    start = clock();
    int a = rand()%255;
    int b = rand()%255;
    int c = rand()%255;
    struct color couleur = {a,b,c};
    struct color white = {255,255,255};
    int n = 0;
    struct point O = {10, 424};
    //struct point
    struct picture fougere = new_pic(taillex,tailley);
    for(int i = 0; i<fougere.height; i++){
        for(int j = 0; j<fougere.width; j++){
            set_pixel(fougere,j,i,white);
        }
    }
    recursivite_devantopti(fougere,O,0,scale,couleur,n,1);
    save_pic(fougere,"fougere.ppm");
    free(fougere.pixel);
    // Obtenir le temps d'arrêt
    end = clock();

    // Calculer le temps écoulé en secondes
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Afficher le temps d'exécution
    printf("Temps d'execution : %.6f secondes\n", cpu_time_used);
    //Version initiale
    //Pour n = 15, on a  1.616672 secondes, 1.687434
    //Pour n = 16, on a  4.489257 secondes, 4.439246
    //Pour n = 17, on a 12.201812 secondes
    //Pour n = 18, on a 35.730829 secondes
    //Pour n = 19, on a 110.629473 secondes
    //Pour n = 20, on a 332.843410 secondes
    //Pour n = 21, on a 962.208853 secondes
    //Pour n = 22, on a 2880 seconds
    //Pour n = 23, 8903 seconds
    //Version optimisée
    ////////////////////////////////////////////////////////////////
    //pas = 1
    //n=15,  0.331147 s
    //n=16, 0.344836 s
    //n=17,0.349569 s
    //n = 18, 0.404237 s
    //n=19, 0.401690 s
    //n= 20,0.457686 s
    //n=21, 0.597474 s
    //n=25,4.962793 s
    //n=26,9.364234 s
    //n=27, 18.483976 s
    //n=28, 36.836083 s
    //n=29, 72.791340 s
    ////////////////////////////////////////////////////////////////
    //pas = 2
    //n=15,0.319623 s
    //n=20, 0.328752 s
    //n=25,0.415717 s
    //n=30,1.603619 s
    //n=35,17.341674 s
    //n=36,29.096037 s
    //n=37,49.130844 s
    //n=38, 85.315097 s
    ////////////////////////////////
    //pas = 3
    //n=15,  0.325388 s
    //n=20,  0.309948 s
    //n=25,  0.325650 s
    //n=30,  0.405966 s
    //n=35,  1.102068 s
    //n=40,  7.230840 s
    //n=41,  10.098332 s
    //n=42,  15.405791 s
    //n=43,  23.635064 s
    //n=44,  36.849929 s
    //n=45,  62.443762 s
    ///////////////////
    //pas = 4
    //n=15, 0.322302 s
    //n=20, 0.304270 s
    //n=25, 0.319111 s
    //n=30, 0.363637 s
    //n=35, 0.422020 s
    //n=40,  0.913839 s
    //n=45, 4.076704 s
    //n=50, 27.533862 s
    //n=51, 37.631931 s
    //n=52, 51.229920 s
    //n=53, 74.701445 s
    ////////////////////
    //pas = 5
    //n=15, 0.331595 s
    //n=20, 0.325551 s
    //n=25, 0.331904 s
    //n=30, 0.329586 s
    //n=35, 0.370755 s
    //n=40, 0.499660 s
    //n=45, 0.875873 s
    //n=50, 3.330401 s
    //n=55, 15.230208 s
    //n=56, 20.854270 s
    //n=57, 28.793322 s
    //n=58, 39.861761 s
    return 0;
}
