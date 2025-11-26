#define ROTATE_SPEED 5
/*
* Przykładowy zestaw poleceń
*/
Begin_Parallel_Actions
Set Podstawa 0 0 0 0 0 45 // Polozenie obiektu A
Set Podstawa.Ramie1 0 0 0 0 0 0 // Polozenie obiektu B
Set Podstawa.Ramie1.Ramie2 0 0 0 0 0 0
End_Parallel_Actions
Pause 3000
Rotate Podstawa.Ramie1 OY ROTATE_SPEED 90
Pause 3000 /* Zawieszenie dzialania na 5 sek. */
Move Podstawa 1 2
Pause 3000 /* Zawieszenie dzialania na 3 sek. */
Rotate Podstawa.Ramie1 OY ROTATE_SPEED 45
Pause 3000 /* Zawieszenie dzialania na 3 sek. */
Rotate Podstawa.Ramie1.Ramie2 OY 36 360
Pause 3000 /* Zawieszenie dzialania na 3 sek. */
Rotate Podstawa.Ramie1.Ramie2 OX 36 360
Pause 3000 /* Zawieszenie dzialania na 3 sek. */