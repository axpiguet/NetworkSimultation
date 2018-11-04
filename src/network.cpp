#include "network.h"
#include "random.h"
#include <random>
#include <algorithm>
#include <iostream>


using namespace std;


/*! 
    Resizes the list of nodes (\ref values) and also resets all values.
    After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
 */
    void Network::resize(const size_t& size){
		
		values.clear();
		RandomNumbers r;
		
		for(size_t i(0) ; i < values.size() ; ++i){
				values[i] = r.normal(0 , 1);  
			}
		
		if(values.size() < size){
			
			for(size_t i(values.size()) ; i < size ; ++i){
				values.push_back(r.normal(0 , 1));  
			}
			
		}else {
			
			while(values.size() != size){  
				values.pop_back();
			}
			
		}
	
		for(multimap<size_t, size_t>::const_iterator node = links.begin() ; node != links.end() ; ++node){
			
			if((node->first >=values.size()) or (node->second >=values.size())){  
				links.erase(node); 
			}
		}
			
	}    
	
	
/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted
 */
    bool Network::add_link(const size_t& a , const size_t& b ){  
		
		size_t size(values.size());
		
		if((a<size) and (b<size) and (a>=0) and (b>=0) and (a!=b)){
			
			links.insert( pair<size_t, size_t>(a,b));   
			links.insert( pair<size_t, size_t>(b,a));  	
			return true;
			
		}
		
		return false;
	}
	
	
/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen),
  where *degree(n)* is Poisson-distributed.
  All previous links are erased first.
  @param[in] mean_deg the average of the Poisson distribution,
  @param[out] number of links created.
*/
    size_t Network::random_connect(const double& mean){   
		
		links.clear();
		random_device rd;
		size_t numlink(0);
		RandomNumbers r(rd());
		vector<int> indicesRandom(values.size());
		
		for(size_t n(0) ; n < values.size(); ++n){   
			
			int nbLink;   
			
			do{
				nbLink = r.poisson(mean);
				
			}while((nbLink<0) or (nbLink>=values.size()));
				
			r.uniform_int(indicesRandom, 0, (values.size()-1) );
				
			for(size_t i(0) ; i < nbLink ; ++i){  
					
				size_t IndexNewlink(indicesRandom[i]);
				
				if(add_link(n , IndexNewlink)){
					++numlink;
				}
			}
		}
		
		return numlink;
	}
	
	
	/*! Resets node values with a vector of *n* new values.
  If the current size is *s* and *n<s* then only the first *n* values are changed.
  If *n>s* then only *s* values are used. 
  The internal \ref values vector keeps the same size.
  @param[in] vector of new node values,
  @param[out] number of nodes succesfully reset.
 */

    size_t Network::set_values(const std::vector<double>& newNodes){
		
		size_t success(0);
		size_t indiceMax(values.size());
		
		if(newNodes.size() < indiceMax){
				
				indiceMax = newNodes.size();
		}
		
		for(size_t i(0) ; i<indiceMax ; ++i){
			values[i] = newNodes[i]; 
			++success;    
		}
		return success;
	}
	
	
/*! Number of nodes */
    size_t Network::size() const{
		
		return values.size();
	}
	
	
	
/*! Degree (number of links) of node no *n* */
    size_t Network::degree(const size_t &_n) const{
			
		if(_n >=values.size()){
			cerr << "out_of_range index" << endl;
			return 0; 
		}
		return links.count(_n);		
	}
	
	
/*! Value of node no *n* */
    double Network::value(const size_t &_n) const{
		
		if(_n < values.size()){
			return values[_n];    
		}
		
		cerr << "node not found" << endl;
		return 0; 
	}
	
	
	
/*! All node values in descending order */
    std::vector<double> Network::sorted_values() const{
		
		std::vector<double> sorted(values);
		
		sort(sorted.begin() , sorted.end());
		reverse(sorted.begin() , sorted.end());
		
		return sorted;
	}
	
	
/*! All neighbors (linked) nodes of node no *n* */
    std::vector<size_t> Network::neighbors(const size_t& n) const{
		
		std::vector<size_t> linkedto;
	
		for(multimap<size_t, size_t>::const_iterator node = links.begin() ; node != links.end() ; ++node){
				
			if(node->first == n){
					
				linkedto.push_back(node->second);
			}
		}
		return linkedto;
}
