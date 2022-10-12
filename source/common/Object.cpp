//////////////////////////////////////////////////////////////////////////////
//
//  --- Object.cpp ---
//  Created by downRian Summa
//
//////////////////////////////////////////////////////////////////////////////


#include "common.h"

/*vec3 Vec3(vec4 v) {

	return vec3(v.x, v.y, v.z);
}

vec4 Vec4(vec3 v) {

	return vec4(v.x, v.y, v.z, 0);
}*/

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Sphere::intersect(vec4 p0, vec4 V) {

	IntersectionValues result;

	//TODO: Ray-sphere setup
	vec4 center = vec4(this -> center.x, this -> center.y, this -> center.z, 1.0);

	result.t = raySphereIntersection(p0, V);
	result.P = p0 + V.operator*(result.t);
	result.N = normalize(result.P.operator-(center));

	return result;
}

/* -------------------------------------------------------------------------- */
/* ------ Ray = p0 + t*V  sphere at origin center and radius radius    : Find t ------- */
double Sphere::raySphereIntersection(vec4 p0, vec4 V) {

	double t = std::numeric_limits< double >::infinity();

	//TODO: Ray-sphere intersection;
	double a, b, c, delta, t1, t2;

	a = dot(V, V);
	b = 2 * dot(V, (p0.operator-(this -> center)));
	c = dot((p0.operator-(this -> center)), (p0.operator-(this -> center))) - pow(this -> radius, 2);

	delta = pow(b, 2) - (4 * a * c);

	if (delta == 0) {

		t = (-b) / (2 * a);
	}
	else if (delta > 0) {

		t1 = (-b - sqrt(delta)) / (2 * a);
		t2 = (-b + sqrt(delta)) / (2 * a);

		if ((t1 < t2) && (t1 > 0)) {

			t = t1;
		}

		else if ((t1 > t2) && (t2 > 0)) {

			t = t2;
		}
	}

	return t;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Square::intersect(vec4 p0, vec4 V) {

	IntersectionValues result;

	//TODO: Ray-square setup
	result.t = raySquareIntersection(INVC * p0, INVC * V);
	result.N = this -> normal;
	result.P = p0 + V.operator*(result.t);

	return result;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
double Square::raySquareIntersection(vec4 p0, vec4 V) {

	double t; //= std::numeric_limits< double >::infinity();

	//TODO: Ray-square intersection;

	vec3 a = vec3(this -> point.x, this -> point.y, this -> point.z);
	vec3 n = vec3(this -> normal.x, this -> normal.y, this -> normal.z);

	vec3 p03 = vec3(p0.x, p0.y, p0.z);
	vec3 V3 = vec3(V.x, V.y, V.z);

	double D = dot(a, n);
	double first = dot(p03, n);
	double second = dot(V3, n);


	if (second != 0 && ((D - first) / second) > 0) {

		t = (D - first) / second;
	}

	else {

		return std::numeric_limits< double >::infinity();
	}


	//Récupération des points :
	vec3 upL = vec3(this -> mesh.vertices[5].x, this -> mesh.vertices[5].y, this -> mesh.vertices[5].z); //Up left point
	vec3 upR = vec3(this -> mesh.vertices[1].x, this -> mesh.vertices[1].y, this -> mesh.vertices[1].z); //Up right point
	vec3 downL = vec3(this -> mesh.vertices[0].x, this -> mesh.vertices[0].y, this -> mesh.vertices[0].z); //Down left point
	vec3 downR = vec3(this -> mesh.vertices[2].x, this -> mesh.vertices[2].y, this -> mesh.vertices[2].z); //Down right point

	//Produit vectoriel :
	vec3 R = p03 + t * V3;

	vec3 pUp = cross(upL - upR, R - upL);
	vec3 pDown = cross(downR - downL, R - downR);
	vec3 pRight = cross(upR - downR, R - upR);
	vec3 pLeft = cross(downL - upL, R - downL);

	if ((dot(n, pUp) <= 0 && dot(n, pDown) <= 0 && dot(n, pRight) <= 0 && dot(n, pLeft) <= 0) || (dot(n, pUp) >= 0 && dot(n, pDown) >= 0 && dot(n, pRight) >= 0 && dot(n, pLeft) >= 0)) {
		
		return t;
	}

	else {

		return std::numeric_limits< double >::infinity();
	}

}

/*OLD VERSION

vec4 dl, dr, ur, ul;

dl = mesh.vertices[0];
dr = mesh.vertices[2];
ur = mesh.vertices[1];
ul = mesh.vertices[5];

vec3 u;
u.x = ur.x - ul.x;
u.y = ur.y - ul.y;
u.z = ur.z - ul.z;

vec3 d;
d.x = dl.x - dr.x;
d.y = dl.y - dr.y;
d.z = dl.z - dr.z;

vec3 l;
l.x = ul.x - dl.x;
l.y = ul.y - dl.y;
l.z = ul.z - dl.z;

vec3 r;
r.x = dr.x - ur.x;
r.y = dr.y - ur.y;
r.z = dr.z - ur.z;

vec3 pt;

pt.x = point.x;
pt.y = point.y;
pt.z = point.z;

vec3 v1;

v1.x = V.x;
v1.y = V.y;
v1.z = V.z;

vec3 p;

p.x = p0.x;
p.y = p0.y;
p.z = p0.z;

vec3 norm = cross(u, l);

if (dot(normal, v1) == 0) {

	return std::numeric_limits< double >::infinity();
}

t = (dot(pt, normal) - dot(p, normal)) / dot(v1, normal);

if (t < 0) {

	return std::numeric_limits< double >::infinity();
}

else {

	vec3 vecRand, prod;

	vecRand.x = p.x - ul.x;
	vecRand.y = p.y - ul.y;
	vecRand.z = p.z - ul.z;

	prod = cross(vecRand, u);

	if (dot(norm, prod) < 0) {

		return std::numeric_limits< double >::infinity();
	}

	vecRand.x = p.x - dr.x;
	vecRand.y = p.y - dr.y;
	vecRand.z = p.z - dr.z;

	prod = cross(vecRand, d);

	if (dot(norm, prod) < 0) {

		return std::numeric_limits< double >::infinity();
	}

	vecRand.x = p.x - dl.x;
	vecRand.y = p.y - dl.y;
	vecRand.z = p.z - dl.z;

	prod = cross(vecRand, l);

	if (dot(norm, prod) < 0) {

		return std::numeric_limits< double >::infinity();
	}

	vecRand.x = p.x - ur.x;
	vecRand.y = p.y - ur.y;
	vecRand.z = p.z - ur.z;

	prod = cross(vecRand, r);

	if (dot(norm, prod) < 0) {

		return std::numeric_limits< double >::infinity();
	}
}

return t;


}
	}*/