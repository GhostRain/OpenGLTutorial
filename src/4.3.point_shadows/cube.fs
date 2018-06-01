#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
	vec3 fragToLight = fragPos - lightPos;
    // 取得最近点的深度
    float closestDepth = texture(shadowMap, fragToLight).r;
	closestDepth *= far_plane;
    // 取得当前片元在光源视角下的深度
	float currentDepth = length(fragToLight);

	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	//float bias = 0.05;
    // 检查当前片元是否在阴影中
    //float shadow = 0.0;
	//float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

	//vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	//for(int x = -1; x <= 1; ++x)
	//{
	//	for(int y = -1; y <= 1; ++y)
	//	{
	//		float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
	//		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
	//	}    
	//}
	//shadow /= 9.0;
	//FragColor = vec4(vec3(closestDepth / far_plane), 1.0);

	float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fragPos);
	float diskRadius = 0.05;
	for(int i = 0; i < samples; ++i)
	{
	    float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
	    closestDepth *= far_plane;
	    if(currentDepth - bias > closestDepth)
	        shadow += 1.0;
	}
	shadow /= float(samples);
    return shadow;
}

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    // Ambient
    vec3 ambient = 0.15 * color;
    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // 计算阴影
    float shadow = ShadowCalculation(fs_in.FragPos);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    

    FragColor = vec4(lighting, 1.0f);
}