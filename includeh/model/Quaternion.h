
/* 
 * File:   Quaternion.h
 * Author: savins
 *
 * Created on 9 de enero de 2016, 22:00
 */

#ifndef QUATERNION_H
#define QUATERNION_H

#include "Point3DDouble.h"
#include <iostream> 

class Quaternion {
public:
    Quaternion();
    Quaternion(double w, double x, double y, double z);
    Quaternion static conjugate(Quaternion& a);
    Quaternion static point3DToQuaternion(Point3DDouble a);
    Quaternion static XYZToQuaternion(double x, double y, double z);
    Quaternion static result(Quaternion q, Quaternion p) ;
    double w;
    double x;
    double y;
    double z;
  
    virtual ~Quaternion();
private:

};


Quaternion operator+(Quaternion& a, Quaternion& b);
Quaternion operator-(Quaternion& a, Quaternion& b);
Quaternion operator*(Quaternion& a, Quaternion& b);

#endif /* QUATERNION_H */

