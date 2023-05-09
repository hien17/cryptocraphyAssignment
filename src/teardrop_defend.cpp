#include <vector>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <arpa/inet.h>

struct iphdr {
    uint16_t frag_off;
    uint16_t frag_more;
    uint16_t tot_len;
    uint16_t id;
    // other fields
};

class IPReassembler {
public:
    IPReassembler() : maxFragments(32), timeout(10) {}
    virtual ~IPReassembler() {}

    void addFragment(const void* data, size_t len) {
        const iphdr* iph = reinterpret_cast<const iphdr*>(data);
        uint16_t fragOff = ntohs(iph->frag_off);
        uint16_t fragMore = ntohs(iph->frag_more);
        uint16_t fragId = ntohs(iph->id);
        uint16_t totLen = ntohs(iph->tot_len);
        // std::cout<<fragOff<<" "<<fragMore<<" "<<fragId<<" "<<totLen<<std::endl;
        if (fragOff == 0) {
            // This is the first fragment, reset the state
            reassembled.clear();
            expectedId= fragId;
            expectedLen=totLen;
            reassembled.reserve(expectedLen);
            std::cout<<"First packet!"<<std::endl;
        }else{
            if (fragId != expectedId || fragOff != expectedLen || reassembled.size()!=0) {
                // This fragment does not match the expected reassembled packet
                // Drop the packet and reset the state
                std::cout<<"This fragment does not match the expected reassembled packet!"<<std::endl;
                
                reassembled.clear();
                return;
            }
    
            if (reassembled.size() + len > reassembled.capacity()) {
                // We are running out of buffer space, drop the packet
                std::cout<<"We are running out of buffer space, drop the packet!"<<std::endl;
                return;
            }
    
            // Copy the payload of the fragment into the reassembled packet buffer
            reassembled.insert(reassembled.end(), static_cast<const char*>(data) + sizeof(iphdr), static_cast<const char*>(data) + len);
        }
        if (fragMore == 0) {
            // This is the last fragment, complete the reassembly
            // TODO: handle the reassembled packet
            reassembled.clear();
            std::cout<<"Done reassembling!";
        }
    }

    void setTimeout(int seconds) {
        timeout = seconds;
    }

    void setMaxFragments(size_t max) {
        maxFragments = max;
    }

private:
    std::vector<char> reassembled;
    uint16_t expectedId = 0;
    uint16_t expectedLen = 0;
    size_t maxFragments;
    int timeout;
};

int main() {
    // Test Case 1: Reassemble a packet with two fragments
    // Create an IPReassembler object
    IPReassembler reassembler;
    
    // Create two legit fragments 
    iphdr fragment1 = {htons(0), htons(1),  htons(1000), htons(12345)}; //Offset, More, Length, ID
    iphdr fragment2 = {htons(1000), htons(0),  htons(1000), htons(12345)};
    
    // Add the first fragment
    reassembler.addFragment(&fragment1, sizeof(fragment1));
    
    // Add the second fragment
    reassembler.addFragment(&fragment2, sizeof(fragment2));
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
	
	// Test Case 2: Reassemble a packet with two fragments
	// Create two fragments where 2nd offset < 1st length
    fragment1 = {htons(0), htons(1),  htons(1000), htons(12345)};
    fragment2 = {htons(900), htons(0),  htons(1000), htons(12345)};
   
    // Add the first fragment
    reassembler.addFragment(&fragment1, sizeof(fragment1));
    
    // Add the second fragment
    reassembler.addFragment(&fragment2, sizeof(fragment2));

    return 0;
}