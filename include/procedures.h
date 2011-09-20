#include "sqlmodules.h"
#ifndef PROCEDURES_H
#define PROCEDURES_H

bool EditChetClienta(int ID_cl, int VidOperacii, double summa, QString Date);
double GetOstatokNaSchete(int ID_cl, QString Date);

#endif // PROCEDURES_H
