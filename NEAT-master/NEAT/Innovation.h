#ifndef _INNOVATION_H_
#define _INNOVATION_H_

namespace NEAT {

    enum innovType {
        NEWNODE = 0,
        NEWLINK = 1
    };

    /**
     * This Innovation class serves as a way to record innovations
     * specifically, so that an innovation in one genome can be
     * compared with other innovations in the same epoch, and if they
     * are the same innovation, they can both be assigned the same
     * innovation number.
     *
     * This class can encode innovations that represent a new link
     * forming, or a new node being added.  In each case, two
     * nodes fully specify the innovation and where it must have
     * occured.  (Between them)
     */
    class Innovation {
    private:
        enum innovType {
            NEWNODE = 0,
            NEWLINK = 1
        };

    public:
        /*Either NEWNODE or NEWLINK*/
        innovType innovationType;

        /**Two nodes specify where the innovation took place*/
        int nodeInId;
        int nodeOutId;

        /**The number assigned to the innovation*/
        double innovationNum1;
        /** If this is a new node innovation, then there are 2 innovations (links) added for the new node */
        double innovationNum2;

        /** If a link is added, this is its weight */
        double newWeight;
        /** If a link is added, this is its connected trait */
        int newTraitNum;

        /** If a new node was created, this is its node_id */
        int newNodeId;

        /** If a new node was created, this is the innovnum of the gene's link it is being stuck inside */
        double oldInnovNum;

        bool recurFlag;

        /**Constructor for the new node case*/
        Innovation(int nin, int nout, double num1, double num2, int newid, double oldinnov);

        /**Constructor for new link case*/
        Innovation(int nin, int nout, double num1, double w, int t);

        /**Constructor for a recur link*/
        Innovation(int nin, int nout, double num1, double w, int t, bool recur);

    };

}

#endif
