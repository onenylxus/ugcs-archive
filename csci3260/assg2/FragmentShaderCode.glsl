#version 430

struct Material {
	sampler2D txtr;
	float shininess;
};

struct DirLight {
	vec3 direction;
	float intensity;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

////////////////////////////////////////////////////////////////

in vec2 UV;
in vec3 normalPos;
in vec3 fragPos;
in vec4 shadowPos;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLight[4];
uniform SpotLight spotLight;
uniform Material material;
uniform sampler2D depthMap;

out vec4 color;

////////////////////////////////////////////////////////////////

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, float visibility) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient;
	vec3 diffuse = light.intensity * light.diffuse * diff * visibility;
	vec3 specular = light.intensity * light.specular * spec * visibility;
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float visibility) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 ambient = light.ambient * attenuation;
	vec3 diffuse = light.diffuse * diff * attenuation * visibility;
	vec3 specular = light.specular * spec * attenuation * visibility;
	return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float visibility) {
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * attenuation;
	vec3 diffuse = light.diffuse * diff * attenuation * intensity * visibility;
	vec3 specular = light.specular * spec * attenuation * intensity * visibility;
	return ambient + diffuse + specular;
}

////////////////////////////////////////////////////////////////

void main() {
	vec3 norm = normalize(normalPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	float visibility = texture(depthMap, shadowPos.xy).z < shadowPos.z ? 1.0 : 0.5;

	vec3 result = calcDirLight(dirLight, norm, viewDir, 1); // Still I failed to do shadows correctly
	for (int i = 0; i < 4; i++) {
		result += calcPointLight(pointLight[i], norm, fragPos, viewDir, 1);
	}
	result += calcSpotLight(spotLight, norm, fragPos, viewDir, 1);
	color = vec4(result * vec3(texture(material.txtr, UV)), 1);
}
