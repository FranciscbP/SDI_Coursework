/*
 * async_publish.cpp
 *
 *  Created on: 28 Nov 2020
 *      Author: ntu-user
 */

// async_publish.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// It's an example of how to send messages as an MQTT publisher using the
// C++ asynchronous client interface.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker
//  - Publishing messages
//  - Last will and testament
//  - Using asynchronous tokens
//  - Implementing callbacks and action listeners
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
#include "headers/publisher.h"

const std::string DFLT_SERVER_ADDRESS	{ "tcp://localhost:1883" };
const std::string DFLT_CLIENT_ID		{ "eclipse_publisher" };

const char* LWT_PAYLOAD = "Last will and testament.";

const int  QOS = 1;

const auto TIMEOUT = std::chrono::seconds(10);

/////////////////////////////////////////////////////////////////////////////

/**
 * A callback class for use with the main MQTT client.
 */
class callback : public virtual mqtt::callback
{
public:
    void connection_lost(const string& cause) override {
        cout << "\nConnection lost" << endl;
        if (!cause.empty())
            cout << "\tcause: " << cause << endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A base action listener.
 */
class action_listener : public virtual mqtt::iaction_listener
{
protected:
    void on_failure(const mqtt::token& tok) override {
        cout << "\tListener failure for token: "
            << tok.get_message_id() << endl;
    }

    void on_success(const mqtt::token& tok) override {
        cout << "\tListener success for token: "
            << tok.get_message_id() << endl;
    }
};

/////////////////////////////////////////////////////////////////////////////

/**
 * A derived action listener for publish events.
 */
class delivery_action_listener : public action_listener
{
    atomic<bool> done_;

    void on_failure(const mqtt::token& tok) override {
        action_listener::on_failure(tok);
        done_ = true;
    }

    void on_success(const mqtt::token& tok) override {
        action_listener::on_success(tok);
        done_ = true;
    }

public:
    delivery_action_listener() : done_(false) {}
    bool is_done() const { return done_; }
};

/////////////////////////////////////////////////////////////////////////////

int publisher (const char* message, const char* topic, string clientID)
{
    mqtt::async_client client(DFLT_SERVER_ADDRESS, clientID);
    callback cb;
    client.set_callback(cb);

    mqtt::connect_options conopts;
    mqtt::message willmsg(topic, LWT_PAYLOAD, 1, true);
    mqtt::will_options will(willmsg);
    conopts.set_will(will);

    try {
        mqtt::token_ptr conntok = client.connect(conopts);
        conntok->wait();

        // First use a message pointer.
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, message);
        pubmsg->set_qos(QOS);
        client.publish(pubmsg)->wait_for(TIMEOUT);
        }
        catch (const mqtt::exception& exc) {
            cerr << exc.what() << endl;
            return 1;
        }
    return 0;
}




