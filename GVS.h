#ifndef GVS_H
#define GVS_H
namespace CiliaSDK
{
	namespace GVS
	{
		//Constant Values
		const unsigned int MAX_NUMBER_OF_CILIAS = 256;
		const unsigned int NUMBER_OF_SURROUND_POSITIONS = 256;
		const unsigned int NUMBER_OF_SMELLS_PER_CILIA = 6;
		const unsigned int NUMBER_OF_LIGHTS_PER_CILIA = 6;
		const unsigned int SMELLS_OFFSET = 1;
		const unsigned int LIGHT_OFFSET = 7;
		const unsigned int SIZE_OF_CILIA_CONTENTS = 13;//surround position + smells + lights
	}
}
#endif