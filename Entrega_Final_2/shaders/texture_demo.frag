#version 330
#define MAX_LIGHTS 9  // Use 9 lights

uniform sampler2D texmap;
uniform sampler2D texmap1;
uniform sampler2D texmap2;
uniform sampler2D texmap3;
uniform sampler2D texmap4;
uniform sampler2D texmap5;
uniform sampler2D texmap6;
uniform sampler2D texmap7;
uniform sampler2D texmap8;
uniform sampler2D texmap9;

uniform int isCarLife;

uniform int texMode;
out vec4 colorOut;
struct Light {
    vec4 position;    
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
	float constantAtt;
    float linearAtt;
	float quadraticAtt;
	int isEnabled;
	float cutOff;
	vec4 spotDirection;
};
struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

uniform Light lights[MAX_LIGHTS];
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
	//vec3 l = normalize(DataIn.lightDir);
	vec3 e = normalize(DataIn.eye);

	float att, dist;
	int i;
	vec3 lightDirection, spotDir;

	for(i = 0; i < MAX_LIGHTS; i++) {
	
		/* compute the distance to the light source to a varying variable*/
		if(lights[i].position.w == 1.0f) { //Point light or Spot light
			lightDirection = vec3(lights[i].position) + DataIn.eye;
			spotDir = normalize(vec3(-lights[i].spotDirection));
		}
		else {  //Directional Light
			lightDirection = vec3(lights[i].position);
		}

		if(lights[i].isEnabled == 0) {
			lightDirection = vec3(0.0);
		}

		vec3 l = normalize(lightDirection);
		dist = length(lightDirection);

		if (dot(spotDir,l) <= lights[i].cutOff) {
			lightDirection = vec3(0.0);
		}

		float intensity = max(dot(n,l), 0.0);
		if (lights[i].position.w == 1.0f && intensity > 0.0) {

			vec3 h = normalize(l + e);
			float intSpec = max(dot(h,n), 0.0);
			spec = lights[i].specular * mat.specular * pow(intSpec, mat.shininess);
		}

		att = 1.0 / (lights[i].constantAtt + lights[i].linearAtt * dist + lights[i].quadraticAtt * dist * dist);
	
		if(texMode == 0) // modulate diffuse color with texel color
		{
			texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
			colorOut += max(att * (intensity * mat.diffuse * lights[i].diffuse * texel + spec),mat.ambient * texel);
		}
		else if (texMode == 1) // diffuse color is replaced by texel color, with specular area or ambient (0.1*texel)
		{
			texel = texture(texmap2, DataIn.tex_coord);  // texel from stone.tga
			colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
		}
		else if(texMode == 2) // multitexturing
		{
			texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
			texel1 = texture(texmap1, DataIn.tex_coord);  // texel from checker.tga
			//colorOut = texel * texel1;
			colorOut += max(att * (intensity*lights[i].diffuse*texel*texel1 + spec), 0.1*texel*texel1);
		}
		else if(texMode == 3) // modulate diffuse color with texel color
		{
			texel = texture(texmap3, DataIn.tex_coord);  // texel from orange1.tga
			colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
			//colorOut = texel;
		}
		else if(texMode == 4) // modulate diffuse color with texel color
		{
			texel = texture(texmap4, DataIn.tex_coord);  // texel from gameover.tga
			colorOut = texel;
			return;
		}
		else if(texMode == 5) // modulate diffuse color with texel color
		{
			texel = texture(texmap5, DataIn.tex_coord);  // texel from pause.tga
			colorOut = texel;
			return;
		}
		else if(texMode == 6) // modulate diffuse color with texel color
		{
			texel = texture(texmap6, DataIn.tex_coord);  // texel from manteiga.tga
			colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
			//colorOut = texel;
		}
		else if(texMode == 7) // modulate diffuse color with texel color
		{
			texel = texture(texmap7, DataIn.tex_coord);  // texel from cheerio.tga
			colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
			//colorOut = texel;
		}
		else if(texMode == 8) // modulate diffuse color with texel color
		{
			texel = texture(texmap8, DataIn.tex_coord);  // texel from candle.tga
			colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
			//colorOut = texel;
		}

	}

	colorOut += mat.emissive;
	colorOut = colorOut / MAX_LIGHTS;
	colorOut.w = 1.0f;

//colorOut = max(intensity * mat.diffuse + spec, mat.ambient)* //texture(texmap, DataIn.tex_coord) * texture(texmap1, DataIn.tex_coord);

}

/*
	vec4 spec = vec4(0.0);
	vec4 texel, texel1;

	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(DataIn.lightDir);
	vec3 e = normalize(DataIn.eye);

	float intensity = max(dot(n,l), 0.0);
	if (intensity > 0.0) {
		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = mat.specular * pow(intSpec, mat.shininess);
	}

	
	if(texMode == 0) // modulate diffuse color with texel color
	{
		texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
		colorOut = max(intensity * mat.diffuse * texel + spec,mat.ambient * texel);
	}
	else if (texMode == 1) // diffuse color is replaced by texel color, with specular area or ambient (0.1*texel)
	{
		texel = texture(texmap2, DataIn.tex_coord);  // texel from stone.tga
		colorOut = max(intensity*texel + spec, 0.1*texel);
	}
	else if(texMode == 2) // multitexturing
	{
		texel = texture(texmap2, DataIn.tex_coord);  // texel from lighwood.tga
		texel1 = texture(texmap1, DataIn.tex_coord);  // texel from checker.tga
		colorOut = texel * texel1;
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
	else if(texMode == 6) // modulate diffuse color with texel color
	{
		texel = texture(texmap6, DataIn.tex_coord);  // texel from manteiga.tga
		colorOut = texel;
	}
	else if(texMode == 7) // modulate diffuse color with texel color
	{
		texel = texture(texmap7, DataIn.tex_coord);  // texel from cheerio.tga
		colorOut = texel;
	}
	else if(texMode == 8) // modulate diffuse color with texel color
	{
		texel = texture(texmap8, DataIn.tex_coord);  // texel from cheerio.tga
		colorOut = texel;
	}

//colorOut = max(intensity * mat.diffuse + spec, mat.ambient)* //texture(texmap, DataIn.tex_coord) * texture(texmap1, DataIn.tex_coord);

}
*/