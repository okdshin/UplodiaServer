#pragma once
//RequestHeaderParser:20130913
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdint.h>

namespace uploadia
{

class RequestHeader{
public:
	RequestHeader(int header_size) : header_size_(header_size){}
	
	int GetHeaderSize()const{
		return header_size_;
	}

	void AddPair(const std::string& key, const std::string& value){
		request_map_.insert(std::pair<std::string, std::string>(key, value));	
	}

	std::string Get(const std::string& key){
		std::map<std::string, std::string>::const_iterator iter = 
			request_map_.find(key);
		if(iter == request_map_.end()){
			return "";	
		}
		return iter->second;
	}

	friend std::ostream& operator<<(std::ostream& os, const RequestHeader& request){
		os << "HeaderSize=" << request.header_size_;
		std::map<std::string, std::string>::const_iterator begin = 
			request.request_map_.begin();
		os << "{\"" << begin->first << "\":\"" << begin->second << "\"";
		++begin;
		for(std::map<std::string, std::string>::const_iterator iter = begin;
				iter != request.request_map_.end(); ++iter){
			os << ", \"" << iter->first << "\":\"" << iter->second << "\"";
		}
		os << "}";
		return os;
	}

private:
	int header_size_;
	std::map<std::string, std::string> request_map_;
};

class RequestHeaderParser{
public:
    RequestHeaderParser(){}
	RequestHeader Parse(const std::vector<uint8_t>& request_vect){
		std::vector<std::vector<uint8_t> > lines;
		std::vector<uint8_t>::const_iterator before_iter = request_vect.begin();
		int header_size = 0;
		while(true){
			std::vector<uint8_t>::const_iterator iter = 
				std::find(before_iter, request_vect.end(), static_cast<uint8_t>('\r'));
			while(*(iter+1) != '\n'){
				iter = std::find(before_iter, request_vect.end(), 
					static_cast<uint8_t>('\r'));
			}
			if(before_iter == iter){
				header_size = 2+iter - request_vect.begin();
				break;	
			}
			lines.push_back(std::vector<uint8_t>(before_iter, iter));
			before_iter = iter+2;
		}
		/*
		for(unsigned int i = 0; i < lines.size(); ++i){
			std::cout << i << ":" << std::string(lines.at(i).begin(), lines.at(i).end()) << std::endl;
		}
		*/
		RequestHeader request(header_size);
		std::string method_str;
		if(lines.front().front() == static_cast<uint8_t>('G')){
			method_str = 
				std::string(lines.front().begin(), lines.front().begin()+3);
		}
		else if(lines.front().front() == static_cast<uint8_t>('P')){
			method_str = 
				std::string(lines.front().begin(), lines.front().begin()+4);	
		}
		request.AddPair("Method", method_str);
		for(unsigned int i = 1; i < lines.size(); ++i){
			std::vector<uint8_t>::iterator iter = 
				std::find(lines.at(i).begin(), lines.at(i).end(), 
					static_cast<uint8_t>(':'));
			while(*(iter+1) != ' '){
				iter = std::find(iter, lines.at(i).end(), 
					static_cast<uint8_t>(':'));
			}
			request.AddPair(
				std::string(lines.at(i).begin(), iter),
				std::string(iter+2, lines.at(i).end()));
		}
		return request;
	}

    ~RequestHeaderParser(){}

private:

};
}

