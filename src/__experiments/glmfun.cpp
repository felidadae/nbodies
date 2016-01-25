#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>


int main () {
	glm::mat4 myMatrix = glm::translate(glm::vec3(15.0f, 0.0f, 0.0f));
	glm::vec4 myVector(15.0f, 10.0f, 10.0f, 1.0f);
	glm::vec4 transformedVector = myMatrix * myVector; // guess the result	

	std::cout << glm::to_string(myMatrix) << std::endl;
	std::cout << glm::to_string(transformedVector) << std::endl;

	return 0;
}
