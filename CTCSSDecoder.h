#ifndef CTCSSDecoder_H
#define CTCSSDecoder_H
#include "Arduino.h"

	class CTCSSDecoder {
		
		
		private:
			volatile bool measureFlag = 0;
			volatile unsigned long timpPrimImpuls = 0;
			volatile unsigned long timpUltimImpuls = 0;
			volatile unsigned long numImpuls = 0;
			uint8_t int_pin = 0;
			bool detected = 0;
			
			static CTCSSDecoder* int_inst;
			void(*detected_callback)( void ) = NULL;
			void(*undetected_callback)( void ) = NULL;
			
			static void Counter( void );
	
		public:
			
			void begin( uint8_t, void(*detected_callback)( void ), void(*undetected_callback)( void ) );
			void listen( uint16_t, uint16_t=100 );
			uint16_t getTone( uint8_t );
	};

#endif