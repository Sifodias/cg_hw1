#include "Vertice.h"

CVertice::CVertice(const vec3 & point) :
	point(point), imported(false) {}

CVertice::CVertice(const vec3 & point, const vec3 & normal) :
	point(point), imported_normal(normal), imported(true) {}

bool CVertice::has_imported_normal()
{
	return imported;
}