//
// Created by rob on 10/10/16.
//

#include "GreenRoomOneAction.h"

GreenRoomOneAction::GreenRoomOneAction(ItemTable *iList, Command *commands) : AbstractRoomAction(iList,commands) {}

GreenRoomOneAction::GreenRoomOneAction(ItemTable *iList) : AbstractRoomAction(iList) {}

ActionResults *GreenRoomOneAction::Close() {

    return new ActionResults(CURRENT, "Nothing to Close.");
}

ActionResults *GreenRoomOneAction::Open() {

    return new ActionResults(CURRENT, "You can't open that here.");
}

ActionResults *GreenRoomOneAction::Touch() {

return new ActionResults(CURRENT, "No touching.");
}


ActionResults *GreenRoomOneAction::Rest() {
    return new ActionResults(CURRENT,"You get some much needed shut eye.");
}

ActionResults *GreenRoomOneAction::Throw() {

    std::string information;

    if (itemList->getValue(commands->getMainItem())->getLocation() != BACKPACK) {
        information = "You can not throw an item that you do not currently "
                "have in your backpack. Pick up item in order to throw it.";
        return new ActionResults(CURRENT, information);

    }

    switch (commands->getMainItem()) {
        case FOX_TOKEN :
            if (getIsGooseHere()) {
                information = "You throw the fox token at the goose. fox "
                        "You see faint translucent tendrils reach out of the rock,"
                        "trying to attach to the goose token, as if"
                        "attempting to surround and consume it. The tendrils dissapear in a puff"
                        "when the token hits the floor.";
            } else {
                if (commands->getActedOnItem() == NOTHING) {
                    information = "You throw the fox token with all your might. It barely "
                            "makes it from your hand before being"
                            "pulled to the ground. Throwing this token will not do much for you.";
                } else {
                    information = "Throwing the token does nothing";
                }
            }
            break;
        default:
            break;

        case GOOSE_TOKEN:
            if (isBeanToken()) {
                information = "You throw the Goose Token at the Bean Token."
                        "You see a flash of predatory excitement as the token"
                        "lands next to the Bean Token.";
            } else if (commands->getActedOnItem() == NOTHING) {

                information = "You throw the fox token with all your might. It barely "
                        "makes it from your hand before being"
                        "pulled to the ground. Throwing this token will not do much for you.";

            } else if (commands->getActedOnItem() == FOX_TOKEN && itemInRoom(FOX_TOKEN)) {
                information = "A shriek fills the room as you throw the Token."
                        "It appears to be from the goose token.";
            }
            break;

        case BEAN_TOKEN :
            if (commands->getActedOnItem() == GOOSE_TOKEN && itemInRoom(GOOSE_TOKEN)) {
                information = "The bean token crosses the path of the goose token and lands with "
                        "an audible, dead, thud. The goose token ";
            }
            if (commands->getActedOnItem() == NOTHING) {
                information = "The bean token leaves your hand and falls to the ground, listless.";
            }
    }

    return new ActionResults(CURRENT, information);
}

ActionResults *GreenRoomOneAction::Look() {

    std::string information;

    if (itemInRoom(commands->getMainItem()) ||
        itemList->getValue(commands->getMainItem())->getLocation()
        != BACKPACK) {
        information = "You can not look at an  item that you do not currently "
                "have in your backpack and is not"
                "in the room.";
        return new ActionResults(CURRENT, information);

    }

    switch (commands->getMainItem()) {
        case FOX_TOKEN : {
            information = "You see a token of a toothy fox, spun around in a circle. "
                    "A backdrop of a chicken coop is in the background.";
            break;
        }
        case GOOSE_TOKEN : {
            information = "A fat goose adorns this token.";
            break;
        }
        case BEAN_TOKEN : {
            information = "A giant black bean adorns this token.";
            break;
        }
        default : {
            information = "You look at the  " + itemList->getValue(commands->getMainItem())->getItemName() + ". "
                "It looks nice.";

        }
    }

    return new ActionResults(CURRENT, information);

}

ActionResults *GreenRoomOneAction::Go() {

    if (commands->getMainItem() == NORTH) {
        return new ActionResults(CURRENT, "You go north.");
    }
    if (commands->getMainItem() == SOUTH) {
        return new ActionResults(CURRENT, "You go south.");
    }
    if (commands->getMainItem() == EAST) {
        return new ActionResults(CURRENT, "You go east.");
    }
    if (commands->getMainItem() == WEST) {
        return new ActionResults(CURRENT, "The path is blocked.\n");
    }

    return new ActionResults(CURRENT,"You can't go there.");
}

ActionResults  *    GreenRoomOneAction::Drop() {
    std::string information;
    itemType item = commands->getMainItem();

    if(itemList->getValue(item)->getLocation() == BACKPACK) {
        dropItem(item);
        information = "Dropped the item " + itemList->getValue(item)->getItemName();
    } else {
        information = "Can not drop an item that isn't in your back pack.";
    }

    return new ActionResults(CURRENT, information);
}

void GreenRoomOneAction::dropItem(const itemType &item) const {
    itemList->getValue(item)->setLocation(itemList->getValue(PLAYER)->getLocation());
}

ActionResults *GreenRoomOneAction::Use() {
    std::string information;
    itemType item = commands->getMainItem();

    switch(item) {
        case G_BOAT :
            information = "You take the boat.";
            takeBoat();
            if(foxEatsGoose()) {
                information = "You take the boat. The fox token consumed the goose token."
                        "all tokens have been reset. Try again!";
                resetTokens();
            }
            else if(gooseEatsBean()) {
                information = "You take the boat. The goose token consumed the bean token."
                        "All tokens have been reset. Try again!";
                resetTokens();
            }
            break;
        default:
            information = "This item can not be used here.";
        }
    return new ActionResults(CURRENT, information);
    }


void GreenRoomOneAction::takeBoat() const {
    if (itemList->getValue(PLAYER)->getLocation() == G_ROOM1_SIDE1) {
                itemList->getValue(PLAYER)->setLocation(G_ROOM1_SIDE2);
            }
            else {
                itemList->getValue(PLAYER)->setLocation(G_ROOM1_SIDE1);
            }
}

void GreenRoomOneAction::resetTokens() const {
    itemList->getValue(FOX_TOKEN)->setLocation(G_ROOM1_SIDE1);
    itemList->getValue(GOOSE_TOKEN)->setLocation(G_ROOM1_SIDE1);
    itemList->getValue(BEAN_TOKEN)->setLocation(G_ROOM1_SIDE1);
}

bool GreenRoomOneAction::gooseEatsBean() const {
    return itemList->getValue(GOOSE_TOKEN)->getLocation() == itemList->getValue(BEAN_TOKEN)->getLocation()
           && itemList->getValue(PLAYER)->getLocation() != itemList->getValue(BEAN_TOKEN)->getLocation();
}

bool GreenRoomOneAction::foxEatsGoose() const {
    return itemList->getValue(FOX_TOKEN)->getLocation() == itemList->getValue(GOOSE_TOKEN)->getLocation()
           && itemList->getValue(PLAYER)->getLocation() != itemList->getValue(GOOSE_TOKEN)->getLocation();
}

ActionResults *GreenRoomOneAction::Pick() {
    std::string information;

    if (itemInRoom(commands->getMainItem()) ||
        itemList->getValue(commands->getMainItem())->getLocation()
        != BACKPACK) {
        information = "You can not look at an  item that you do not currently "
                "have in your backpack and is not"
                "in the room.";
        return new ActionResults(CURRENT, information);

    }

    switch (commands->getMainItem()) {
        case FOX_TOKEN :
        case GOOSE_TOKEN :
        case BEAN_TOKEN :
            if (!hasAToken() && itemInRoom(commands->getMainItem())) {
                information = "You pick up the the " +
                        itemList->getValue(commands->getMainItem())->getItemName();
                itemList->getValue(commands->getMainItem())->setLocation(BACKPACK);
            } else {
                information = "Only one token allowed to be picked up at a time.";
            }
            break;
        default:
            if (itemInRoom(commands->getMainItem())) {
                itemList->getValue(commands->getMainItem())->setLocation(BACKPACK);
                information = "You pick up the " + itemList->getValue(commands->getMainItem())->getItemName();
            } else {
                information = "You cant pick up an item that isnt here.";
            }
            break;
    }

    return new ActionResults(CURRENT, information);


}

bool GreenRoomOneAction::hasAToken() const {
    return itemList->getValue(GOOSE_TOKEN)->getLocation() == BACKPACK
           || itemList->getValue(BEAN_TOKEN)->getLocation() == BACKPACK
           || itemList->getValue(FOX_TOKEN)->getLocation() == BACKPACK;
}


bool GreenRoomOneAction::itemInRoom(itemType item) const {
    return itemList->getValue(item)->getLocation()
           == itemList->getValue(PLAYER)->getLocation();
}

bool GreenRoomOneAction::isBeanToken() const {
    return commands->getActedOnItem() == BEAN_TOKEN &&
           itemList->getValue(BEAN_TOKEN)->getLocation()
           == itemList->getValue(PLAYER)->getLocation();
}

bool GreenRoomOneAction::getIsGooseHere() const {
    return commands->getActedOnItem() == GOOSE_TOKEN &&
           itemList->getValue(GOOSE_TOKEN)->getLocation() == itemList->getValue(PLAYER)->getLocation();
}