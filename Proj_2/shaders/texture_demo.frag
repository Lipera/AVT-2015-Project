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

uniform bool isFogActive;
uniform float CameraHeight;

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
	bool isSpot;
	float cutOff;
	float spotExponent;
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
	vec2 tex_coord;
	vec3 vertex_pos;
} DataIn;

const vec3 fogColor = vec3(0.5, 0.5,0.5);
const float FogDensity = 0.05;

void main() {

	vec3 scatteredLight = vec3(0.0); // total of diffuse light
	vec3 reflectedLight = vec3(0.0); //total of specular light

	vec3 n = normalize(DataIn.normal);
	vec3 e = normalize(DataIn.eye);
	vec3 lightDirection;
	float attenuation;
	int i;

	vec3 texel, texel1;

	//Textures where the light contribution is not taken into account
	if(texMode == 4) // texel color
	{
		texel = vec3(texture(texmap4, DataIn.tex_coord));  // texel from gameover.tga
		colorOut = vec4(texel, mat.diffuse.w);
		return;
	}
	else if(texMode == 5) // texel color
	{
		texel = vec3(texture(texmap5, DataIn.tex_coord));  // texel from pause.tga
		colorOut = vec4(texel, mat.diffuse.w);
		return;
	}
	

	// loop over all the lights
	for (i = 0; i < MAX_LIGHTS; i++) {
		if (lights[i].isEnabled == 0) {
			continue;
		}
		
		if(lights[i].position.w == 0.0) { //Directional light
			lightDirection = normalize(vec3(lights[i].position));
			attenuation = 1.0;
		}
		
		else { //Spotlight or Pointlight
			vec3 ld = vec3(lights[i].position) - DataIn.vertex_pos;
			float lightDistance = length(ld);
			//lightDirection = lightDirection / lightDistance;

			attenuation = 1.0 / (lights[i].constantAtt + lights[i].linearAtt * lightDistance + lights[i].quadraticAtt * lightDistance * lightDistance);

			lightDirection = normalize(ld);
			
			if (lights[i].isSpot) { //Spotlight
				vec3 sd = normalize(vec3(-lights[i].spotDirection));
				float spotCos = max(dot(lightDirection, sd), 0.0);
				
				if (spotCos < cos(radians(lights[i].cutOff))) {
					attenuation = 0.0;
				}

				else {
					attenuation *= pow(spotCos, lights[i].spotExponent);
				} 
			}
			
		}
		
		float intensity = max(dot(n,lightDirection), 0.0);
        vec3 diff = vec3(lights[i].diffuse * intensity);
        vec3 spec = vec3(0.0);
               
        if (intensity > 0.0) {
                vec3 h = normalize(lightDirection + e);
                float intSpec = max(dot(h,n), 0.0);
                spec = vec3(lights[i].specular * pow(intSpec, mat.shininess));
        }
 
        scatteredLight += attenuation * diff * vec3(mat.diffuse);
        reflectedLight += attenuation * spec * vec3(mat.specular); 
	}
	
	colorOut = vec4(vec3(mat.emissive),0.0);
	//Textures where the light contribution is taken into account
	if(texMode == 0) // modulate diffuse color with texel color
	{
		texel = vec3(texture(texmap2, DataIn.tex_coord));  // texel from lighwood.tga

		/*
		if(isCarLife == 1) {
			colorOut += vec4(max(vec3(mat.diffuse) * texel + vec3(mat.specular), vec3(mat.ambient) * texel), mat.diffuse.w);
			return;
		}
		*/

		colorOut += vec4(max(scatteredLight * texel + reflectedLight, vec3(mat.ambient) * texel), mat.diffuse.w);
	}
	else if (texMode == 1) // diffuse color is replaced by texel color, with specular area or ambient (0.1*texel)
	{
		texel = vec3(texture(texmap2, DataIn.tex_coord));  // texel from stone.tga
		colorOut += vec4(max((scatteredLight/(vec3(mat.diffuse))) * texel + reflectedLight, 0.1*texel), mat.diffuse.w);
	}
	else if(texMode == 2) // multitexturing
	{
		texel = vec3(texture(texmap2, DataIn.tex_coord));  // texel from lighwood.tga
		texel1 = vec3(texture(texmap1, DataIn.tex_coord));  // texel from checker.tga
		//colorOut = texel * texel1;
		colorOut += vec4(max(scatteredLight * texel * texel1 + reflectedLight, 0.1*texel*texel1), mat.diffuse.w);
	}
	else if(texMode == 3) // modulate diffuse color with texel color
	{
		texel = vec3(texture(texmap3, DataIn.tex_coord));  // texel from orange1.tga
		colorOut += vec4(max(scatteredLight * texel + reflectedLight, 0.1*texel), mat.diffuse.w);
		//colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
	}
	else if(texMode == 6) // modulate diffuse color with texel color
	{
		texel = vec3(texture(texmap6, DataIn.tex_coord));  // texel from manteiga.tga
		colorOut += vec4(max(scatteredLight * texel + reflectedLight, 0.1*texel), mat.diffuse.w);
		//colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
	}
	else if(texMode == 7) // modulate diffuse color with texel color
	{
		texel = vec3(texture(texmap7, DataIn.tex_coord));  // texel from cheerio.tga
		colorOut += vec4(max(scatteredLight * texel + reflectedLight, 0.1*texel), mat.diffuse.w);
		//colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
	}
	else if(texMode == 8) // modulate diffuse color with texel color
	{
		texel = vec3(texture(texmap8, DataIn.tex_coord));  // texel from candle.tga
		colorOut += vec4(max(scatteredLight * texel + reflectedLight, 0.1*texel), mat.diffuse.w);
		//colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), 0.1*texel);
	}
	else if(texMode == 9) // modulate diffuse color with texel color
	{
		texel = vec3(texture(texmap9, DataIn.tex_coord));  // texel from tree.tga
		colorOut += vec4(max(scatteredLight + reflectedLight, vec3(mat.ambient)), mat.diffuse.w);
		//colorOut += max(att * (intensity*lights[i].diffuse*texel + spec), vec3(mat.ambient));
	}/*else if (texMode == 10)  {  // tree texture for billboard
		texel = texture(texmap, DataIn.tex_coord);  		if(texel.a == 0.0) discard;
		else
			colorOut = vec4(max(intensity*texel.rgb + spec, 0.1*texel.rgb), texel.a);
	}*/


	//----------------------------------------FOG -------------------------------------------------


	if(isFogActive) {
		//distance
		float dist = 0;
		float fogFactor = 0;

		//range based fog
		dist = length(DataIn.vertex_pos); 

		
		// exponential fog
		fogFactor = 1.0 /exp((dist * FogDensity) * (dist * FogDensity));
		fogFactor = clamp( fogFactor, 0.0, 1.0 );

		colorOut = vec4(mix(fogColor, vec3(colorOut), fogFactor), colorOut.a);
		
		/*
		//my camera y is 10.0. you can change it or pass it as a uniform
		float be = (CameraHeight - DataIn.vertex_pos.y) * 0.004;//0.004 is just a factor; change it if you want
		float bi = (CameraHeight - DataIn.vertex_pos.y) * 0.001;//0.001 is just a factor; change it if you want

		float ext = exp(-dist * be);
		float insc = exp(-dist * bi);

		colorOut = vec4(vec3(colorOut) * ext + fogColor * (1 - insc), colorOut.a);
		*/
	}

}
