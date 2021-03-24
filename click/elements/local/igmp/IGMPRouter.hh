//
// Created by thomas on 17/03/21.
//

#ifndef CLICK_IGMPROUTERELEMENT_H
#define CLICK_IGMPROUTERELEMENT_H


//TODO REF-4.2.13: accept 0.0.0.0
//TODO REF-6: accept 224.0.0.22

// terminated group membership report -> query network before deleting group

class IGMPRouter : public Element {
public:
    const char* class_name() const override { return "IGMPRouter"; }
    const char* port_count() const override { return "1/1"; }
    const char* processing() const override { return PUSH; }

    int configure(Vector<String>&, ErrorHandler*) override;

    void push(int, Packet*) override;
    static void processReport(Packet* packet);

private:
    IGMPRouterState* state;
};


#endif //CLICK_IGMPROUTERELEMENT_H
