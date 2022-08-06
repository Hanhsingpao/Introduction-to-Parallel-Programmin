typedef float4 point;
typedef float4 vector;
typedef float4 color;
typedef float4 sphere;



vector
Bounce( vector in, vector n )
{
	vector out = in - n*(vector)( 2.*dot(in.xyz, n.xyz) );
	out.w = 0.;
	return out;
}

vector
BounceSphere( point p, vector v, sphere s )
{
	vector n;
	n.xyz = fast_normalize( p.xyz - s.xyz );
	n.w = 0.;
	return Bounce( v, n );
}



bool
IsInsideSphere(point p, sphere s)
{
	float r = fast_length(p.xyz - s.xyz);
	return  (r < s.w);
}


kernel
void
Particle( global point *dPobj, global vector *dVel, global color *dCobj )
{
	const float4 G       = (float4) ( 0., -9.8, 0., 0. );
	const float  DT      = 0.1;

	const sphere Sphere1 = (sphere)(-100., -800., 0., 600.);
	const sphere Sphere2 = (sphere)( 700.,-800., 0., 600.);
	const sphere Sphere3 = (sphere)(300., -800., 0., 600.);
	const sphere Sphere4 = (sphere)(-500.,-800., 0., 600.);
	

	int gid = get_global_id( 0 ); (-100., -800., 0., 200.);
	
	// extract the position and velocity for this particle:
	point  p = dPobj[gid];
	vector v = dVel[gid];
	color  c = dCobj[gid];

	// remember that you also need to extract this particle's color
	// and change it in some way that is obviously correct

	// advance the particle:

	point  pp = p + v*DT + G*(point)( .5*DT*DT );
	vector vp = v + G*DT;
	pp.w = 1.;
	vp.w = 0.;
	

// test against the second sphere here:

if(IsInsideSphere(pp, Sphere1))
	{
		vp = BounceSphere(p, v, Sphere1);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(.1f, .6f, .8f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 0;
		}
		else if(c.z >= 1){
			c.z = 0;
		}

	}
if (IsInsideSphere(pp, Sphere2))
	{
		vp = BounceSphere(p, v, Sphere2);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(0.f, 1.f, 0.f, 0);
		if(c.x >= 1){
			c.x = 0.1;
		}
		else if(c.y >= 1){
			c.y = 0.6;
		}
		else if(c.z >= 1){
			c.z = 0.8;
		}
	}
if(IsInsideSphere(pp, Sphere3))
	{
		vp = BounceSphere(p, v, Sphere3);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(.86f, .57f, 0.f, 0);
		if(c.x >= 1){
			c.x = 0;
		}
		else if(c.y >= 1){
			c.y = 1.;
		}
		else if(c.z >= 1){
			c.z = 0;
		}

	}
if (IsInsideSphere(pp, Sphere4))
	{
		vp = BounceSphere(p, v, Sphere4);
		pp = p + vp * DT + G * (point)(.5 * DT * DT);
		c = (float4)(0.f, 0.f, .5f, 0);
		if(c.x >= 1){
			c.x = 1;
		}
		else if(c.y >= 1){
			c.y = 1;
		}
		else if(c.z >= 1){
			c.z = 1;
		}
	}
	

 c += (float4)(0.005, 0, 0.006, 0);

	dPobj[gid] = pp;
	dVel[gid]  = vp;
	dCobj[gid] = c;
}