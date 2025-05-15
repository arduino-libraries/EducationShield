/*
	Copyright (C) Arduino SA

	This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
	distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

#include "EducationShield.h"

Joystick::Joystick(int x, int y){
	this->x=x;
	this->y=y;
}

int Joystick::getX(){
    int v = analogRead(x);
    if(v<300) return(-1);
    else if(v>700) return(1);
    else return(0);

}
int Joystick::getY(){
    int v = analogRead(y);
    if(v<300) return(-1);
    else if(v>700) return(1);
    else return(0);
}
