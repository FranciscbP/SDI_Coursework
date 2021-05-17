// subscriber.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an MQTT subscriber using the C++ asynchronous client
// interface, employing callbacks to receive messages and status updates.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker.
//  - Subscribing to a topic
//  - Receiving messages through the callback API
//  - Receiving network disconnect updates and attempting manual reconnects.
//  - Using a "clean session" and manually re-subscribing to topics on
//    reconnect.
//

/*******************************************************************************
 * Copyright (c) 2013-2017 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/

#include "../headers/header.h"

const string SERVER_ADDRESS("tcp://localhost:1883");
const string CLIENT_ID("eclipse_subscriber");
const string TOPIC("ChatRoom");

const int	QOS = 1;
const int	N_RETRY_ATTEMPTS = 5;

string getMessages(int);
int updateMessages(int,string);

string getMessages(int chatID)
{
    //Connect to Database
     MYSQL* conn;
     MYSQL_ROW row;
     MYSQL_RES* res;
     conn = mysql_init(0);
	 conn = mysql_real_connect(conn, "127.0.0.1", "root", "secret", "sdi_project", 3306, 0, 0);

     if(conn)
     {
         stringstream selectQuery;
         selectQuery << "Select * from chats where chatID ='" << chatID << "'";

         string queryStr = selectQuery.str();

         //Convert String to Const Char
         const char* q = queryStr.c_str();

         int exeQuery = mysql_query(conn, q);
         
         if(!exeQuery)
         {        	 
             res = mysql_store_result(conn);
             row = mysql_fetch_row(res);

             if (row != nullptr)
             {
            	 if(row[1] != nullptr)
            	 {
                     //Return Username
                     char *chr = row[1];
                     string text(chr);
                     return text;
            	 }
            	 else
            	 {
            		 return "";
            	 }

             }
             else
             {
                 return 0;
             }
         }
         else
         {
             return 0;
         }
     }
     else
     {
         return 0;
     }

     mysql_close(conn);
}


int updateMessages(int chatID,string message)
{		
	string previousMessages = getMessages(chatID);
		
	string updated = previousMessages + "\n"+ message;
			
	//Connect to Database	    
	    MYSQL* conn;
	    conn = mysql_init(0);
	    conn = mysql_real_connect(conn, "127.0.0.1", "root", "secret", "sdi_project", 3306, 0, 0);

	    if(conn)
	    {
	        stringstream insertQuery;
	        insertQuery << "UPDATE chats SET text = '"<< updated <<"' where chatID = '"<< chatID <<"'";

	        string queryStr = insertQuery.str();

	        //Convert String to Const Char
	        const char* q = queryStr.c_str();
	        int exeQuery = mysql_query(conn, q);

	        if(exeQuery == 0)
	        {
	            return 1;
	        }
	        else
	        {
	            return 0;
	        }
	    }
	    else
	    {
	        return 0;
	    }

	    mysql_close(conn);

}


/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener
{
	string name_;

	void on_failure(const mqtt::token& tok) override {
		cout << name_ << " failure";
		if (tok.get_message_id() != 0)
			cout << " for token: [" << tok.get_message_id() << "]" << endl;
		cout << endl;
	}

	void on_success(const mqtt::token& tok) override {
		cout << name_ << " success";
		if (tok.get_message_id() != 0)
			cout << " for token: [" << tok.get_message_id() << "]" << endl;
		auto top = tok.get_topics();
		if (top && !top->empty())
			cout << "\ttoken topic: '" << (*top)[0] << "', ..." << endl;
		cout << endl;
	}

public:
	action_listener(const string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
	// An action listener to display the result of actions.
	action_listener subListener_;

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void reconnect() {
		this_thread::sleep_for(chrono::milliseconds(2500));
		try {
			cli_.connect(connOpts_, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			cerr << "Error: " << exc.what() << endl;
			exit(1);
		}
	}

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {
		cout << "Connection attempt failed" << endl;
		if (++nretry_ > N_RETRY_ATTEMPTS)
			exit(1);
		reconnect();
	}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override {}

	// (Re)connection success
	void connected(const string& cause) override {
		cout << "\nConnection success" << endl;
		cout << "\nSubscribing to topic '" << TOPIC << "'\n"
			<< "\nPress Q<Enter> to quit\n" << endl;

		cli_.subscribe(TOPIC, QOS, nullptr, subListener_);
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const string& cause) override {
		cout << "\nConnection lost" << endl;
		if (!cause.empty())
			cout << "\tcause: " << cause << endl;

		cout << "Reconnecting..." << endl;
		nretry_ = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		
		string messageReceived = msg->to_string();
		
		string chatID = messageReceived.substr(0, messageReceived.find("-"));
		string message = messageReceived.substr(chatID.size() + 1);
		
		cout << "\nChatRoom: '" << chatID << "'" << endl;
		cout << "\tMessage arrived" << endl;
		
		int intChatID = stoi(chatID);
		int updateMsg = updateMessages(intChatID, message);
		
		if(updateMsg != 0)
		{
			cout << "\tMessage Sent to Database" << endl;
		}
		else
		{
			cout << "\tMessage Not Sent to Database" << endl;
		}
		
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
				: nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_session(true);

	mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

	callback cb(client, connOpts);
	client.set_callback(cb);

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		cout << "Connecting to the MQTT server..." << flush;
		client.connect(connOpts, nullptr, cb);
	}
	catch (const mqtt::exception&) {
		cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< SERVER_ADDRESS << "'" << endl;
		return 1;
	}

	// Just block till user tells us to quit.

	while (tolower(cin.get()) != 'q')
		;

	// Disconnect

	try {
		cout << "\nDisconnecting from the MQTT server..." << flush;
		client.disconnect()->wait();
		cout << "OK" << endl;
	}
	catch (const mqtt::exception& exc) {
		cerr << exc.what() << endl;
		return 1;
	}

 	return 0;
}