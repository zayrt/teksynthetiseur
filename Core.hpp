#ifndef		CORE_HPP_
#define		CORE_HPP_

#include <SFML/Audio.hpp>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <"v"tream>
#include <string.h>
#include  "cJSON.h"

#define                     	PATH_INSTRU	"./la.wav"
#define                     	PATH_SOUND	"./test.wav"

class Core
{

private:
  std::map<std::string,float> 	_notes;
  std::map<std::string,float> 	_values;
  cJSON                     		*_tracks;
  std::string										_content;
  std::fstream              		_fs;
  sf::SoundBuffer 							_buffer;

public:
  Core() {
  	_notes["do"] = 261.63;
  	_notes["ré"] = 293.66;
  	_notes["mi"] = 329.63;
  	_notes["fa"] = 349.23;
  	_notes["sol"] = 392.00;
  	_notes["la"] = 440.00;
  	_notes["si"] = 493.88;
  	_values["ronde"] = 4;
  	_values["blanche"] = 2;
  	_values["noir"] = 1;
  	_values["croche"] = 1.0 / 2;
  	_values["double croche"] = 1.0 / 4;
  	_values["triple croche"] = 1.0 / 8;
  }

  ~Core() {}
  
  int                   getFilecontent()
  {
    _fs.open(PATH_SOUND, std::fstream::in | std::fstream::out);
    if (_fs) {
      _fs.seekg (0, _fs.end);
      long                        size = _fs.tellg();
      _fs.seekg (0);
      char*                       buffer = new char[size];
      _fs.read(buffer, size);
      std::string                 str(buffer);
      delete[] buffer;
      _content = str;
      return 0;
    }
    std::cout << "open failed path incorrect" << std::endl;
    return -1;
  }

  void 						  my_wait(double t) {
    clock_t a = clock()+ (t * CLOCKS_PER_SEC);
    while(clock() < a);
	}

		float wait_value = 0;

    for (int j = 0 ; j < cJSON_GetArraySize(note) ; j++) {
    	cJSON             *subnote = cJSON_GetArrayItem(note, j);		
    	cJSON             *n = cJSON_GetObjectItem(subnote, "name");
    	cJSON             *v = cJSON_GetObjectItem(subnote, "value");
     
    	if (n == NULL)
    		std::cout << "There is a note without name" << std::endl;
    	else if (v == NULL)
    		std::cout << "There is a note without value" << std::endl;
    	else {
      	std::string name = n->valuestring;
      	std::string value = v->valuestring;
      	float pitch = _notes[name] /_notes["la"];
     
      	if (!_notes[name])
      		std::cout << "Error note unrecognized: " + name << std::endl;
      	else if (!_values[value])
      		std::cout << "Error value unrecognized: " + value << std::endl;
      	else { 
      		sound.setPitch(pitch);                                                       
  				sound.play();
    			if (_values[value] > wait_value)
    				wait_value = _values[value];
    		}
    	}		
    }
    my_wait((60 / tempo) * wait_value);
	}

  int 						getNotes(cJSON *notes, double tempo) {
    sf::Sound 			sound;

    sound.setBuffer(_buffer);
    sound.setLoop(true);
  	for (int i = 0 ; i < cJSON_GetArraySize(notes) ; i++) {
    	cJSON             *note = cJSON_GetArrayItem(notes, i);
    	
    	play_note(sound, note, tempo);
    }	
  }

  int 						play_track() {
  	if (getFilecontent() == -1)
  		return -1;
  	if (!_buffer.loadFromFile(PATH_INSTRU)) {
      return -1;
    }
  	cJSON               *c = cJSON_Parse(_content.c_str());
  	if (c == NULL) {
  		std::cout << "Json incorrect" << std::endl;
  		return -1;
  	}
  	if ((_tracks = cJSON_GetObjectItem(c, "tracks")) == NULL){
  		std::cout << "There isn't tracks on this JSON" << std::endl;
  		return -1;
  	}
  	for (int i = 0 ; i < cJSON_GetArraySize(_tracks) ; i++) {
        cJSON               *subitem = cJSON_GetArrayItem(_tracks, i);
        cJSON               *tempo = NULL;
        cJSON 							*notes = NULL;

        if ((tempo = cJSON_GetObjectItem(subitem, "tempo")) == NULL) 
        	std::cout << "There is a track without tempo on this JSON" << std::endl;
        else if ((notes = cJSON_GetObjectItem(subitem, "notes")) == NULL)
        	std::cout << "There is a track without notes on this JSON" << std::endl;	
        else {
        	double t = tempo->valuedouble;
        	getNotes(notes, t);
    		}
    }
  }
};

#endif		/* Core_HPP_ */