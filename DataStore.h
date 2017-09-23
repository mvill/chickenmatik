#include <EEPROM.h>

#ifndef DataStore_h
#define DataStore_h

class DataStore {

private:

	const int ADDRESS_UP_HOURS = 0; // uint8_t -> 1 byte
	const int ADDRESS_UP_MINUTES = 1;
	const int ADDRESS_DOWN_HOURS = 2;
	const int ADDRESS_DOWN_MINUTES = 3;
	const int ADDRESS_UP_POSITION = 4; // long -> 4 bytes
	const int ADDRESS_DOWN_POSITION = 8;
	const int ADDRESS_CURRENT_POSITION = 12;

	uint8_t upHours = 0;
	uint8_t upMinutes = 0;
	uint8_t downHours = 0;
	uint8_t downMinutes = 0;
	long upPosition = 0;
	long downPosition = 0;
	long currentPosition = 0;

	void eepromWriteUint8(int address, uint8_t value) {
		EEPROM.write(address, value);
	}

	uint8_t eepromReadUint8(long address) {
		uint8_t value = EEPROM.read(address);
		return value;
	}

	void eepromWriteLong(int address, long value) {
		//Decomposition from a long to 4 bytes by using bitshift.
		//One = Most significant -> Four = Least significant byte
		byte four = (value & 0xFF);
		byte three = ((value >> 8) & 0xFF);
		byte two = ((value >> 16) & 0xFF);
		byte one = ((value >> 24) & 0xFF);

		//Write the 4 bytes into the eeprom memory.
		EEPROM.write(address, four);
		EEPROM.write(address + 1, three);
		EEPROM.write(address + 2, two);
		EEPROM.write(address + 3, one);
	}

	long eepromReadLong(long address) {
		//Read the 4 bytes from the eeprom memory.
		long four = EEPROM.read(address);
		long three = EEPROM.read(address + 1);
		long two = EEPROM.read(address + 2);
		long one = EEPROM.read(address + 3);

		//Return the recomposed long by using bitshift.
		return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF)
				+ ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
	}

public:
	DataStore(){
	}
	uint8_t getDownHours() const {
		return downHours;
	}

	void setDownHours(uint8_t downHours = 0) {
		eepromWriteUint8(ADDRESS_DOWN_HOURS, downHours);
		this->downHours = downHours;
	}

	uint8_t getDownMinutes() const {
		return downMinutes;
	}

	void setDownMinutes(uint8_t downMinutes = 0) {
		eepromWriteUint8(ADDRESS_DOWN_MINUTES, downMinutes);
		this->downMinutes = downMinutes;
	}

	uint8_t getUpHours() const {
		return upHours;
	}

	void setUpHours(uint8_t upHours = 0) {
		eepromWriteUint8(ADDRESS_UP_HOURS, upHours);
		this->upHours = upHours;
	}

	uint8_t getUpMinutes() const {
		return upMinutes;
	}

	void setUpMinutes(uint8_t upMinutes = 0) {
		eepromWriteUint8(ADDRESS_UP_MINUTES, upMinutes);
		this->upMinutes = upMinutes;
	}

	long getDownPosition() const {
		return downPosition;
	}

	void setDownPosition(long downPosition = 0) {
		eepromWriteLong(ADDRESS_DOWN_POSITION, downPosition);
		this->downPosition = downPosition;
	}

	long getUpPosition() const {
		return upPosition;
	}

	void setUpPosition(long upPosition = 0) {
		eepromWriteLong(ADDRESS_UP_POSITION, upPosition);
		this->upPosition = upPosition;
	}

	long getCurrentPosition() const {
		return currentPosition;
	}

	void setCurrentPosition(long currentPosition = 0) {
		eepromWriteLong(ADDRESS_CURRENT_POSITION, currentPosition);
		this->currentPosition = currentPosition;
	}


	void reset(){
		this->setUpHours(0);
		this->setUpMinutes(0);
		this->setDownHours(0);
		this->setDownMinutes(0);
		this->setUpPosition(0);
		this->setDownPosition(0);
		this->setCurrentPosition(0);
	}

	void init(){
		this->upHours = eepromReadUint8(ADDRESS_UP_HOURS);
		this->upMinutes = eepromReadUint8(ADDRESS_UP_MINUTES);
		this->downHours = eepromReadUint8(ADDRESS_DOWN_HOURS);
		this->downMinutes = eepromReadUint8(ADDRESS_DOWN_MINUTES);
		this->upPosition = eepromReadLong(ADDRESS_UP_POSITION);
		this->downPosition = eepromReadLong(ADDRESS_DOWN_POSITION);
		this->currentPosition = eepromReadLong(ADDRESS_CURRENT_POSITION);

//		Serial.println( this->upHours );
//		Serial.println( this->upMinutes );
//		Serial.println( this->downHours );
//		Serial.println( this->downMinutes );
//		Serial.println( this->upPosition );
//		Serial.println( this->downPosition );
//		Serial.println( this->currentPosition );
	}

};

#endif
