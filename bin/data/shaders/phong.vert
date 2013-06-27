#extension GL_EXT_gpu_shader4 : require

varying vec3 normal, lightDir, eyeVec;

uniform float shaderVal1;
uniform float shaderVal2;
uniform float shaderVal3;
uniform float time;

void main( void ){
	
	normal = gl_NormalMatrix * gl_Normal;
	//normal += 0.1 * noise3( 4.2 * gl_Vertex); //make normals a bit dirty
	
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeVec = -vVertex;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex ;
}