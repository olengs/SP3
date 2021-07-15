#include "TRS.h"

TRS TRS::operator+(TRS& rValue)
{
	TRS temp = *this;
	temp.translate += rValue.translate;
	return temp;
}
