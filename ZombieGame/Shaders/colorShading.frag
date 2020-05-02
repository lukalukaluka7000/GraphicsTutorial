#version 130

out vec4 color;

//uniform float time;
uniform sampler2D mySampler;

//test
in vec2 fragmentPosition; // attributi brate
in vec4 fragmentColor;
in vec2 fragmentUV;

void main(){
    //get the color from a texture
    
    //drugi par korinate for our pixel (u,v)->[0,1]
    vec4 textureColor = texture2D(mySampler, fragmentUV);
    
    //color = vec3(1.0, 1.0, 1.0);
    
    color = textureColor * fragmentColor; // color blending
    
    //color = vec4(fragmentColor.r * (sin(fragmentPosition.y  + time) + 1.0) * 0.5,
    //             fragmentColor.g * (300*sin(fragmentPosition.y  + time) + 1.0) * 0.5,
    //             fragmentColor.b * (600*sin(fragmentPosition.x  + 1.0) * 0.5, fragmentColor.a)+ textureColor * time);
                 

    //color = vec4(100*time/(2*time), 0.5, 100*time/(2*time) , fragmentColor.a)* textureColor;
} 