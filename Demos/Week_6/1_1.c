#include <stdio.h>

typedef struct{
   double a;
   double b;
   double c;
} Cuboid;

int setSize(Cuboid *cuboid, double a, double b, double c){
    if(a <= 0 || b <= 0 || c <= 0){
        printf("Hibás input, 0-tól nagyobb hossz kell");
        return 0; //df
    }
    cuboid->a = a;
    cuboid->b = b;
    cuboid->c = c;
    return 1;
}
double calc_volume(Cuboid cuboid){
    return cuboid.a * cuboid.b * cuboid.c;
}

double calc_surface(Cuboid cuboid){
    return 2 *(cuboid.a * cuboid.b + cuboid.a * cuboid.c + cuboid.b * cuboid.c);
}

int vanNegyzet(Cuboid cuboid){
    if(cuboid.a == cuboid.b || cuboid.c == cuboid.b || cuboid.a == cuboid.c){
        return 1;
    }
    return 0;
}

int main(){
    Cuboid cuboid;

    if(setSize(&cuboid, 3.0, 4.0, 5.0)){
        double terfogat = calc_volume(cuboid);
        double felszin = calc_surface(cuboid);
        printf("Térfogat: %.2f\nFelszín: %.2f\n", terfogat, felszin);
        if(vanNegyzet(cuboid)){
            printf("Van négyzet lapja");
        } else{
            printf("Nincs négyzet lapja");
        }
    } else{
        printf("Hibás oldalszámok");
    }
    return 0;
}