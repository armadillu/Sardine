#extension GL_EXT_gpu_shader4 : require

varying vec3 normal, lightDir, eyeVec;

uniform float shaderVal1;
uniform float shaderVal2;
uniform float shaderVal3;
uniform float time;
uniform float specularGain;
uniform float specularClamp;
uniform float specularPow;
uniform float eyeSpecularGain;
uniform float eyeSpecularClamp;
uniform float eyeSpecularPow;

uniform float showInShader;

void main( void ){

	// AMBIENT /////////////////////////////////
	vec4 final_color =
		(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) 
		+
		(gl_LightSource[0].ambient * gl_FrontMaterial.ambient)
		;
	
	vec3 N = normalize( normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = max( dot(N, L), 0.0) ;
	
	// DIFFUSE /////////////////////////////////
	if(lambertTerm > 0.0){
		final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
	}

	vec3 E = normalize(eyeVec);
	vec3 R = (reflect(-L, N));
	vec3 H = normalize (L + E);
	// SPECULAR /////////////////////////////////
	float specular = specularGain * clamp( pow( max(dot(R,E), 0.0), gl_FrontMaterial.shininess * specularPow ), 0.0, specularClamp);
	vec4 specularV = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular;	
	
	// EYE SPECULAR ////////////////////////////
	float eyeSpecular = eyeSpecularGain * clamp( pow( max( dot(L,-N) * dot(E,-N), 0.0), eyeSpecularPow * 100.0), 0.0, eyeSpecularClamp);
	//float eyeSpecular = eyeSpecularGain * pow(clamp(dot(N,H),0.0,eyeSpecularClamp), eyeSpecularPow * 100.0); 
	vec4 eyeSpecularV = gl_LightSource[0].specular * gl_FrontMaterial.specular * eyeSpecular;
				
	if (showInShader == 0.0){
		final_color += specularV;
		final_color += eyeSpecularV;
		gl_FragColor = final_color;
	}else if (showInShader == 1.0){
		gl_FragColor = vec4(N, 1.0);
	}else if (showInShader == 2.0){
		gl_FragColor = vec4(L, 1.0);
	}else if (showInShader == 3.0){
		gl_FragColor = vec4(E, 1.0);
	}else if (showInShader == 4.0){
		gl_FragColor = vec4(R, 1.0);
	}else if (showInShader == 5.0){
		final_color += specularV;
		gl_FragColor = final_color;
	}else if (showInShader == 6.0){
		final_color += eyeSpecularV;
		gl_FragColor = final_color;
	}	
}