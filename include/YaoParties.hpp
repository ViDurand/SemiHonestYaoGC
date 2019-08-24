#pragma once 

#include <boost/thread/thread.hpp>
#include <libscapi/include/comm/Comm.hpp>
#define AES_KEY BC_AES_KEY // AES_KEY is defined both in GarbledBooleanCircuit and in OTSemiHonestExtension
//#define NO_AESNI
#define KEY_SIZE 16

#ifdef NO_AESNI
    #include <libscapi/include/circuits/GarbledBooleanCircuitNoIntrinsics.h>
    #include <libscapi/include/interactive_mid_protocols/OTExtensionLibote.hpp>

#else
    #include <libscapi/include/circuits/GarbledBooleanCircuit.h>

    #ifdef _WIN32
        #include <libscapi/include/interactive_mid_protocols/OTSemiHonestExtension.hpp>
    #else
         #include <libscapi/include/interactive_mid_protocols/OTExtensionBristol.hpp>
    #endif

#endif
#include <libscapi/include/cryptoInfra/Protocol.hpp>
#include <libscapi/include/cryptoInfra/SecurityLevel.hpp>
#include <libscapi/include/circuits/GarbledCircuitFactory.hpp>

#undef AES_KEY
#define AES_KEY OT_AES_KEY

#undef AES_KEY
#include <thread>
#include <libscapi/include/infra/Scanner.hpp>
#include <libscapi/include/infra/ConfigFile.hpp>




struct YaoConfig {
	bool print_output;
	string circuit_type;
	string circuit_file;
	string input_file_1;
	string input_file_2;
	IpAddress sender_ip;
	int sender_port;
	YaoConfig(bool print, string c_file, string input_file_1,
		string input_file_2, string sender_ip_str, int sender_port, string circuit_type) {
		print_output = print;
		circuit_file = c_file;
		this->input_file_1 = input_file_1;
		this->input_file_2 = input_file_2;
		sender_ip = IpAddress::from_string(sender_ip_str);
		this->circuit_type = circuit_type;
		this->sender_port = sender_port;
	};

	YaoConfig(string config_file) { // @suppress("Class members should be properly initialized")
#ifdef _WIN32
		string os = "Windows";
#else
		string os = "Linux";
#endif
		ConfigFile cf(config_file);
		cout<<"after config file"<<endl;
		string str_print_output = cf.Value("", "print_output");
		istringstream(str_print_output) >> std::boolalpha >> print_output;
		string input_section = cf.Value("", "input_section") + "-" + os;
		circuit_file = cf.Value(input_section, "circuit_file");
		input_file_1 = cf.Value(input_section, "input_file_party_1");
		input_file_2 = cf.Value(input_section, "input_file_party_2");
		circuit_type = cf.Value("", "circuit_type");
	}

	YaoConfig() {} // @suppress("Class members should be properly initialized")
};

/**
* This is an implementation of party one of Yao protocol.
*/
class PartyOne : public MPCProtocol, public SemiHonest{
private:
	int id;
	OTBatchSender * otSender;			//The OT object that used in the protocol.

#ifdef NO_AESNI
	GarbledBooleanCircuitNoIntrinsics * circuit;	//The garbled circuit used in the protocol.
    tuple<byte*, byte*, vector<byte> > values;//this tuple includes the input and output keys (block*) and the translation table (vector)
                                        //to be used after filled by garbling the circuit
#else
	GarbledBooleanCircuit* circuit;	//The garbled circuit used in the protocol.
    tuple<block*, block*, vector<byte> > values;//this tuple includes the input and output keys (block*) and the translation table (vector)
    //to be used after filled by garbling the circuit
#endif
	shared_ptr<CommParty> channel;				//The channel between both parties.




	vector<byte> ungarbledInput;				//Inputs for the protocol
	YaoConfig yaoConfig;


	/**
	* Sends p1 input keys to p2.
	* @param ungarbledInput The boolean input of each wire.
	* @param bs The keys for each wire.
	*/
	void sendP1Inputs(byte* ungarbledInput);

	/**
	* Runs OT protocol in order to send p2 the necessary keys without revealing any other information.
	* @param allInputWireValues The keys for each wire.
	*/
	void runOTProtocol();

public:
	/**
	* Constructor that sets the parameters of the OT protocol and creates the garbled circuit.
	* @param channel The channel between both parties.
	* @param bc The boolean circuit that should be garbled.
	* @param mes The encryption scheme to use in the garbled circuit.
	* @param otSender The OT object to use in the protocol.
	* @param inputForTest
	*/
	PartyOne(int argc, char* argv[]);

	~PartyOne() {
#ifdef NO_AESNI
        delete [] get<0>(values);
        delete [] get<1>(values);
#else
        //delete inputs and output block arrays
#endif
		delete circuit;
		delete otSender;


	}

	void setInputs(string inputFileName, int numInputs);

    bool hasOffline() override { return false; }
    bool hasOnline() override { return true; }

    void runOnline() override;

	YaoConfig& getConfig() { return yaoConfig; }

	int getID() {return id;}
};

/**
* This is an implementation of party two of Yao protocol.
*/
class PartyTwo : public MPCProtocol, public SemiHonest{
private:
	int id;
	OTBatchReceiver * otReceiver;			//The OT object that used in the protocol.
#ifdef NO_AESNI
	GarbledBooleanCircuitNoIntrinsics * circuit;	//The garbled circuit used in the protocol.
#else
	GarbledBooleanCircuit* circuit;	//The garbled circuit used in the protocol.
#endif
	shared_ptr<CommParty> channel;				//The channel between both parties.
	byte* p1Inputs;
	int p1InputsSize;
	bool print_output;					// Indicates if to print the output at the end of the execution or not

	vector<byte> circuitOutput;
	vector<byte> ungarbledInput;
	YaoConfig yaoConfig;
	
	/**
	* Compute the garbled circuit.
	* @param otOutput The output from the OT protocol, which are party two inputs.
	*/
	void computeCircuit(OTBatchROutput * otOutput);

	/**
	* Receive the circuit's garbled tables and translation table.
	*/
	void receiveCircuit();
	/**
	* Receives party one input.
	*/
	void receiveP1Inputs();
	/**
	* Run OT protocol in order to get party two input without revealing any information.
	* @param sigmaArr Contains a byte indicates for each input wire which key to get.
	* @return The output from the OT protocol, party two inputs.
	*/
	shared_ptr<OTBatchROutput> runOTProtocol(byte* sigmaArr, int arrSize) {
		//Create an OT input object with the given sigmaArr.
		vector<byte> sigma;
		copy_byte_array_to_byte_vector(sigmaArr, arrSize, sigma, 0);
		int elementSize = 128;
		OTBatchRInput * input = new OTExtensionGeneralRInput(sigma, elementSize);
		//Run the Ot protocol.
		return otReceiver->transfer(input);
	};

public:
	/**
	* Constructor that sets the parameters of the OT protocol and creates the garbled circuit.
	* @param channel The channel between both parties.
	* @param bc The boolean circuit that should be garbled.
	* @param mes The encryption scheme to use in the garbled circuit.
	* @param otSender The OT object to use in the protocol.
	* @param inputForTest
	*/
	PartyTwo(int argc, char* argv[]);

	~PartyTwo() {
		delete circuit;
		delete otReceiver;

	}

	void setInputs(string inputFileName, int numInputs);

	bool hasOffline() override { return false; }
	bool hasOnline() override { return true; }

    void runOnline() override;

	vector<byte> getOutput() {	return circuitOutput; }

	YaoConfig& getConfig() { return yaoConfig; }

	int getID() {return id;}
};
