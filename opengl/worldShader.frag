#version 430

in Data
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
} data;
in vec3 LightVector;
in vec3 CameraVector;

uniform vec3 lightPosition;
uniform sampler2D TexColor;
uniform sampler2D MoonTexColor;
uniform sampler2D TexGrey;
uniform float textureOffset;

out vec4 FragColor;

vec3 ambientReflectenceCoefficient = vec3(0.5f, 0.5f, 0.5f);
vec3 ambientLightColor = vec3(0.6f, 0.6f, 0.6f);
vec3 specularReflectenceCoefficient= vec3(1.0f, 1.0f, 1.0f);
vec3 specularLightColor = vec3(1.0f, 1.0f, 1.0f);
float SpecularExponent = 10;
vec3 diffuseReflectenceCoefficient;
vec3 diffuseLightColor = vec3(1.0f, 1.0f, 1.0f);


void main()
{
    // Calculate texture coordinate based on data.TexCoord
    vec2 textureCoordinate = data.TexCoord;
    vec4 texColor = texture(TexColor, textureCoordinate);

    diffuseReflectenceCoefficient = vec3(texColor.x, texColor.y, texColor.z);

    vec3 lightReflection = reflect(-LightVector, data.Normal);

    float d = max(dot(LightVector, data.Normal), 0.0f);
    float s = pow(max(dot(lightReflection, CameraVector), 0.0f), SpecularExponent);

    vec3 ambient = ambientLightColor * ambientReflectenceCoefficient;
    vec3 diffuse = diffuseLightColor * diffuseReflectenceCoefficient * d;
    vec3 specular = specularLightColor * specularReflectenceCoefficient * s;

    vec3 realColor = texColor.xyz * (ambient + diffuse + specular);

    FragColor = vec4(clamp(realColor, 0.0f, 1.0f), 1.0f);
    //FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
