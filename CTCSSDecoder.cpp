#include "CTCSSDecoder.h"

//Mapa tonów CTCSS
const uint16_t ctcss_map[] PROGMEM = {
  670,
  719,
  744,
  770,
  797,
  825,
  854,
  885,
  915,
  948,
  974,
  1000,
  1035,
  1072,
  1109,
  1148,
  1188,
  1230,
  1273,
  1318,
  1365,
  1413,
  1462,
  1514,
  1567,
  1622,
  1679,
  1738,
  1799,
  1862,
  1928,
  2035,
  2107,
  2181,
  2257,
  2336,
  2418,
  2503
};


CTCSSDecoder* CTCSSDecoder::int_inst = NULL; //Wewnętrzna instancja obiektu klasy

//Metody prywatne
void CTCSSDecoder::Counter( void ){
		if( int_inst ){
			unsigned long now = micros();
				
				if ( int_inst->numImpuls == 1 ){
					int_inst->timpPrimImpuls = now;
				} else {
					int_inst->timpUltimImpuls = now;
				}
			
			++int_inst->numImpuls;
		}
}


//Metody publiczne
void CTCSSDecoder::begin( uint8_t int_pin, void(*detected_callback)( void ), void(*undetected_callback)( void ) ){
	this->int_inst = this;
	this->int_pin = int_pin;
	this->detected_callback = detected_callback;
	this->undetected_callback = undetected_callback;
}

void CTCSSDecoder::listen( uint16_t tone, uint16_t rate ){
	
		if( !tone ) return;
		
	uint32_t curr = millis();
	static uint32_t update = 0;

		if( !this->measureFlag ){
			this->measureFlag = 1;
			attachInterrupt( digitalPinToInterrupt( this->int_pin ), CTCSSDecoder::Counter, RISING );
		}

		if( curr-update >= rate ){
			update = curr;
				
				if( this->measureFlag ){
					this->measureFlag = 0;
					detachInterrupt(0);
					float f = (this->numImpuls < 3) ? 0 : (10000000.0/*996500.0*/ * (float)(this->numImpuls - 2))/(float)(this->timpUltimImpuls - this->timpPrimImpuls);  
					
					f /= 10.0;
					uint16_t fint = f * 10;
					static uint8_t counter = 0;
					int16_t t = tone-fint;
					t = abs( t );
					
						if( t < 10 ){	
								
								if( counter < 4 ) counter += 2;
								else if( !this->detected ){
									this->detected = true;
										if( this->detected_callback ) this->detected_callback();
								}
						
						} else {
								if( counter > 0 ) counter--;
								else if( this->detected ){
									this->detected = false;
										if( this->undetected_callback ) this->undetected_callback();
								}
						}

					this->numImpuls = 0;
					this->timpUltimImpuls = 0;
					this->timpPrimImpuls = 0;
				}
		}
}

uint16_t CTCSSDecoder::getTone( uint8_t id ){
		if( id && id <= (uint16_t)(sizeof(ctcss_map)/sizeof(ctcss_map[0])) ){
			uint16_t ctcss = 0;
			memcpy_P( &ctcss, &ctcss_map[id-1], sizeof( ctcss_map[id-1] ) );
			return ctcss;
		}
return 0;
}