#include "sqlmodules.h"
#include "params.h"

#ifndef PROCEDURES_H
#define PROCEDURES_H

bool EditChetClienta(int ID_cl, int VidOperacii, double summa, QString Date);
double GetOstatokNaSchete(int ID_cl, QString Date);
double GetOstatokNaSklade(int ID_MATERIAL, int vid_zatrat,QString Date, int type_sklad);

#endif // PROCEDURES_H
