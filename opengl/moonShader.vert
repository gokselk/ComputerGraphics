#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTex;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 NormalMatrix;
uniform mat4 MVP;

uniform sampler2D TexColor;
uniform sampler2D TexGrey;
uniform float textureOffset;
uniform float orbitDegree;

uniform float heightFactor;
uniform float imageWidth;
uniform float imageHeight;

out Data
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} data;


out vec3 LightVector;// Vector from Vertex to Light;
out vec3 CameraVector;// Vector from Vertex to Camera;

vec3 initialCenterPos = vec3(0.0f, 2600.0f, 0.0f);

void main()
{
    // get orbitDegree value, compute new x, y coordinates
    // there won't be height in moon shader

    data.TexCoord = vec2(VertexTex.x + textureOffset / 250.f, VertexTex.y);
    float beta = -radians(orbitDegree);
    float cosB = cos(beta);
    float sinB = sin(beta);

    vec3 centerPos = vec3(initialCenterPos.x * cosB - initialCenterPos.y * sinB,
                          initialCenterPos.x * sinB + initialCenterPos.y * cosB,
                          initialCenterPos.z);

    data.Normal = (NormalMatrix * vec4(VertexNormal, 1.0f)).xyz;

    data.Position = centerPos + VertexPosition;

    LightVector = normalize(lightPosition - data.Position);
    CameraVector = normalize(cameraPosition - data.Position);

   // set gl_Position variable correctly to give the transformed vertex position
    gl_Position = MVP * vec4(data.Position, 1.0f);
}