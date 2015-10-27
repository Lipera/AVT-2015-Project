#version 330

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texmap2;
uniform sampler2D texmap3;
uniform sampler2D texmap4;
uniform sampler2D texmap5;

uniform int texMode;
out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};
uniform Materials mat;


in Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
	vec2 tex_coord;
} DataIn;

void main() {

	vec4 spec = vec4(0.0);
	vec4 texel, texel1;

	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(DataIn.lightDir);
	vec3 e = normalize(DataIn.eye);

	float att, dist;

	/*--------------------- TEMPORARIO -----------------------*/
	float constantAtt = 0.2f;
    float linearAtt = 0.2f;
	float quadraticAtt = 0.1f;
	/*--------------------- TEMPORARIO -----------------------*/

	/* compute the distance to the light source to a varying variable*/
    dist = length(DataIn.lightDir);

	float intensity = max(dot(n,l), 0.0);
	if (intensity > 0.0) {
		att = 1.0 / (constantAtt + linearAtt * dist + quadraticAtt * dist * dist);

		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = mat.specular * pow(intSpec, mat.shininess);
	}

	
	if(texMode == 0) // modulate diffuse color with texel color
	{
		texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
		colorOut = max(att * (intensity * mat.diffuse * texel + spec),mat.ambient * texel);
	}
	else if (texMode == 1) // diffuse color is replaced by texel color, with specular area or ambient (0.1*texel)
	{
		texel = texture(texmap2, DataIn.tex_coord);  // texel from stone.tga
		colorOut = max(att * (intensity*texel + spec), 0.1*texel);
	}
	else if(texMode == 2) // multitexturing
	{
		texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
		texel1 = texture(texmap1, DataIn.tex_coord);  // texel from checker.tga
		//colorOut = texel * texel1;
		colorOut = max(att * (intensity*texel*texel1 + spec), 0.1*texel*texel1);
	}
	else if(texMode == 3) // modulate diffuse color with texel color
	{
		texel = texture(texmap3, DataIn.tex_coord);  // texel from orange1.tga
		//colorOut = max(intensity*texel + spec, 0.1*texel);
		colorOut = texel;
	}
	else if(texMode == 4) // modulate diffuse color with texel color
	{
		texel = texture(texmap4, DataIn.tex_coord);  // texel from gameover.tga
		colorOut = texel;
	}
	else if(texMode == 5) // modulate diffuse color with texel color
	{
		texel = texture(texmap5, DataIn.tex_coord);  // texel from pause.tga
		colorOut = texel;
	}

//colorOut = max(intensity * mat.diffuse + spec, mat.ambient)* //texture(texmap, DataIn.tex_coord) * texture(texmap1, DataIn.tex_coord);

}