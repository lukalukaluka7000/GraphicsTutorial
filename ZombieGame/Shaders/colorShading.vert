#version 130

//what are our inputs
in vec2 vertexPosition; // _colorProgram.addAttribute("vertexPosition");
in vec4 vertexColor;    // _colorProgram.addAttribute("vertexColor");
in vec2 vertexUV;       //novi atribut takoder

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragmentUV; //konzistent, saljemo fragmentu

uniform mat4 P;
void main(){
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    

    fragmentPosition = vertexPosition;  //fragment jer saljemo u .frag
    
    fragmentColor = vertexColor;        //fragment jer saljemo u .frag

    fragmentUV = vec2(vertexUV.x, 1.0-vertexUV.y);     //fragment jer saljemo u .frag
}