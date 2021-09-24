#include "actors.h"

//Actor_base:
std::ostream& operator<<(std::ostream& out, const Actor_base& actor) {
    out << actor.getSign();
    return out;
}

//Player:
std::map<Actor*, int> Player::getInventory() { return m_stats.inventory; }

void Player::updateStats(const Actor* item) { m_stats += item->getStats(); }

void Player::addItemToInventory(Actor*& item) {
    updateStats(item);
    if (m_stats.inventory.find(item) != m_stats.inventory.end())
        ++m_stats.inventory[item];
    else
        m_stats.inventory[item] = 1;
}

void Player::showInventory() const {
    if (m_stats.inventory.empty())
        std::cout << "Your inventory is empty.\n";
    else {
        std::cout << "Your inventory contains: ";
        for (const auto& [key, value] : m_stats.inventory) {
            std::cout << key->getName() << " - " << value << " ";
        }
        std::cout << "\n";
    }
}