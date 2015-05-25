#include "map.h"

#include "game.h"
#include "util.h"
#include "constants.h"

#include <algorithm>
#include <functional>
#include <math.h>

#include "easylogging++.h"

const int HEX_SIZE = 60;
const int HEX_OUTLINE_SIZE = 4;
const int HEX_PRECISION = 6;

const int POINT_SIZE = 10;
const int POINT_OUTLINE_SIZE = 1;
const int POINT_PRECISION = 72;

MapObject::MapObject()
    : pos_(),
      texture_(),
      sprite_() {}

MapObject::~MapObject() {}

sf::Vector2f MapObject::get_pos() const {
    return pos_;
}

void MapObject::set_pos(sf::Vector2f new_pos) {
    pos_ = new_pos;
}

void MapObject::Draw(sf::RenderWindow* window) {}

Hex::Hex(Hex* ul, Hex* ur, Hex* l, Hex* r,
    Hex* dl, Hex* dr, int num, int type)
    : up_left(ul),
      up_right(ur),
      left(l),
      right(r),
      down_left(dl),
      down_right(dr),
      num_(num),
      type_(type) {}

//TODO : carefully review it
Hex::~Hex() {
}

void Hex::set_num(int num) {
    this -> num_ = num;
}

int Hex::get_num() const {
    return this -> num_;
}

void Hex::set_type(int type) {
    this -> type_ = type;
}

int Hex::get_type() const {
    return type_;
}

std::string Hex::to_string() const {
    std::string res;
//    if (NULL != up_left)
//        res.append("up_left NON_NULL,");
//    else
//        res.append("up_left NULL, ");
//
//    if (NULL != left)
//        res.append("left NON_NULL, ");
//    else
//        res.append("left NULL, ");
//
//    if (NULL != down_left)
//        res.append("down_left NON_NULL, ");
//    else
//        res.append("down_left NULL, ");
//
//    if (NULL != down_right)
//        res.append("down_right NON_NULL, ");
//    else
//        res.append("down_right NULL, ");
//
//    if (NULL != right)
//        res.append("right NON_NULL, ");
//    else
//        res.append("right NULL, ");
//
//    if (NULL != up_right)
//        res.append("up_right NON_NULL; ");
//    else
//        res.append("up_right NULL; ");

    res.append("type ")
        .append(std::to_string(type_))
        .append(", num ")
        .append(std::to_string(num_));
    return res;
}

void Hex::Click() {
    LOG(INFO) << to_string() << " clicked";
}

bool Hex::OnMouse(sf::Vector2i point) const {
    double sqrt2 = sqrt(2.0);
    sf::Vector2f center = pos_ +
        sf::Vector2f(HEX_SIZE * sqrt2 * .5, HEX_SIZE * sqrt2 * .5);

    double radius = (double) HEX_SIZE;
    return sqrt((center.x - point.x) * (center.x - point.x) +
            (center.y - point.y) * (center.y - point.y)) - radius < EPS + 20;
}

bool Hex::CanBeAdded(Point* point) {
    int index = GetIndexForPoint(point);
    return !points_[(index + 5) % 6] &&
        !points_[(index + 7) % 6];
}

void Hex::AddPoint(Point* point) {
    AddPoint(point, GetIndexForPoint(point));
}

void Hex::AddPoint(Point* point, int index) {
    points_[index] = point;
}

int Hex::GetIndexForPoint(Point* point) {
    Hex* point_hexes[3];
    point -> get_hexes(point_hexes);
    bool matches[6] = {false};
    for (int i = 0; i < 3; ++i) {
        Hex* curr = point_hexes[i];
        if (this == curr)
            continue;
        if (curr == up_left)
            matches[0] = true;
        if (curr == up_right)
            matches[1] = true;
        if (curr == right)
            matches[2] = true;
        if (curr == down_right)
            matches[3] = true;
        if (curr == down_left)
            matches[4] = true;
        if (curr == left)
            matches[5] = true;
    }
    int index = -1;
    for (int i = 0; index == -1 && i < 6; ++i) {
        if (matches[i] && matches[(i + 1) % 6])
            index = i;
    }
    return index;
}

std::vector<int> Hex::GetBoundPlayersIds() const {
    std::vector<int> result;
    for (int i = 0; i < 6; ++i) {
        if (points_[i]) {
            result.push_back(points_[i] -> get_owner_id());
        }
    }
    return result;
}

Point::Point(Hex* f, Hex* s, Hex* t) {
    hexes_.insert(f);
    hexes_.insert(s);
    hexes_.insert(t);
}

Point::~Point() {
    hexes_.clear();
}

Hex** Point::get_hexes(Hex** res) const {
    for (int i = 0; i < 3; ++i)
        res[i] = NULL;
    int k = 0;
    for (std::set<Hex*>::iterator it = hexes_.begin();
            it != hexes_.end(); ++it)
        res[k++] = *it;
    return res;
}

void Point::Click() {
//    std::string log;
//    log.append(std::to_string(first_ -> get_num()))
//        .append(" ")
//        .append(std::to_string(second_ -> get_num()))
//        .append(" ")
//        .append(std::to_string(third_ -> get_num()))
//        .append(" clicked");
//    LOG(INFO) << log;
}

bool Point::OnMouse(sf::Vector2i cursor) const {
    bool res = true;
    for (std::set<Hex*>::iterator it = hexes_.begin();
            it != hexes_.end() && res; ++it)
        res = (*it) -> OnMouse(cursor);
    return res;
}

bool Point::CanBeOwned(const int _id) {
    bool res = false;
    for (std::set<Line*>::iterator it = lines_.begin();
            it != lines_.end() && !res; ++it) {
        int l_id = (*it) -> get_owner_id();
        res = _id == l_id;
    }
    return res;
}

void Point::AddLine(Line* l) {
    lines_.insert(l);
}

int Point::get_owner_id() const {
    return this -> owner_id_;
}

void Point::set_owner_id(int owner_id) {
    this -> owner_id_ = owner_id;
}

std::set<Hex*> Point::get_hexes() {
    return hexes_;
}

Line::Line(Point* f, Point* s, std::set<Hex*> h)
    : first_(f),
      second_(s),
      hexes_(h) {}

Line::~Line() {
    delete first_;
    delete second_;
    hexes_.clear();
}

Line* Line::FromPoints(Point* first, Point* second) {
    std::set<Hex*> first_hexes_ = first -> get_hexes();
    std::set<Hex*> second_hexes_ = second -> get_hexes();
    std::set<Hex*> hexes_;

    std::set_intersection(first_hexes_.begin(), first_hexes_.end(),
            second_hexes_.begin(), second_hexes_.end(),
            std::inserter(hexes_, hexes_.begin()));
    Line* line = NULL;
    if (hexes_.size() == 2)
        line = new Line(first, second, hexes_);
    return line;
}

void Line::Click() {
//    std::string log;
//    log.append(std::to_string(first_ -> get_num()))
//        .append(" ")
//        .append(std::to_string(second_ -> get_num()))
//        .append(" clicked");
//    LOG(INFO) << log;
}

bool Line::OnMouse(sf::Vector2i cursor) const {
    bool res = true;
    for (std::set<Hex*>::iterator it = hexes_.begin();
            it != hexes_.end() && res; ++it)
        res = (*it) -> OnMouse(cursor);
    return res;
}

/**
 * TODO: check in rules if this is correct
 */
bool Line::CheckOwnership(int owner_id) const {
    int fid = first_ -> get_owner_id();
    int sid = second_ -> get_owner_id();
    return (fid == -1 && (second_ -> CanBeOwned(owner_id) || sid == owner_id)) ||
        ((first_ -> CanBeOwned(owner_id) || fid == owner_id) && sid == -1) ||
        ((first_ -> CanBeOwned(owner_id) || fid == owner_id) && (second_ -> CanBeOwned(owner_id) || sid == owner_id));
}

void Line::set_owner_id(int owner_id) {
    this -> owner_id_ = owner_id;
}

int Line::get_owner_id() const {
    return this -> owner_id_;
}

Point** Line::get_points(Point** container) const {
    container[0] = first_;
    container[1] = second_;
    return container;
}

NotificationArea*
        NotificationArea::CreateInstance(std::string font_path) {
    NotificationArea *instance = new NotificationArea;
    if (!instance -> notification_text_font_.loadFromFile(
                font_path)) {
        LOG(ERROR) << "Error loading font";
        return nullptr;
    }
    instance -> notification_text_
        .setFont(instance -> notification_text_font_);
    instance -> notification_text_.setCharacterSize(40);
    instance -> notification_text_.setColor(sf::Color::White);
    instance -> set_pos(sf::Vector2f(.5f * (float) SCREEN_WIDTH,
                10));
    return instance;
}

NotificationArea::NotificationArea()
    : notification_text_(),
      notification_text_font_() {}

void NotificationArea::Click() {}

bool NotificationArea::OnMouse(sf::Vector2i cursor) const {
    return false;
}

void NotificationArea::MakeVisible() {
    MakeVisible(INF_DURATION);
}

void NotificationArea::MakeVisible(int duration) {
    visible_ = true;
    duration_ = duration;
}

void NotificationArea::MakeInvisible() {
    visible_ = false;
    duration_ = 0;
}

void NotificationArea::Draw(sf::RenderWindow* const window) {
    if (visible_) {
        notification_text_.setPosition(pos_);
        window -> draw(notification_text_);
    }
}

void NotificationArea::SetContent(std::string text) {
    notification_text_.setString(text);
    set_pos(sf::Vector2f(.5f * ((float)SCREEN_WIDTH -
                notification_text_.getLocalBounds().width), 10));
}

void NotificationArea::Update() {
    if (visible_ && duration_ != INF_DURATION)
        visible_ = --duration_ != 0;
}

Button* Button::CreateInstance(sf::Vector2f pos,
                               sf::Vector2f b_size) {
    Button* instance = new Button(b_size);
    instance -> pos_ = pos;
    instance -> shape_.setPosition(pos);
    instance -> sprite_.setPosition(pos);
    return instance;
}

Button::Button(sf::Vector2f b_size)
    : shape_(b_size),
      callbacks_() {}

void Button::Click() {
    for (size_t i = 0; i < callbacks_.size(); ++i)
        callbacks_[i]();
}

bool Button::OnMouse(sf::Vector2i cursor) const {
    return shape_.getGlobalBounds()
        .contains((float) cursor.x,
                  (float) cursor.y);
}

void Button::Draw(sf::RenderWindow* window) {
    if (has_texture_) {
        if (OnMouse(sf::Mouse::getPosition(*window)))
            sprite_.setColor(focused_);
        else
            sprite_.setColor(idle_);
        window -> draw(sprite_);
    } else {
        if (OnMouse(sf::Mouse::getPosition(*window)))
            shape_.setFillColor(focused_);
        else
            shape_.setFillColor(idle_);
        window -> draw(shape_);
    }
}

Button* Button::AddCallback(std::function<void()> cb) {
    callbacks_.push_back(cb);
    return this;
}

Button* Button::SetColors(sf::Color idle, sf::Color focused) {
    idle_ = idle;
    focused_ = focused;
    return this;
}

Button* Button::LoadTextureFrom(std::string filename) {
    if (texture_.loadFromFile(filename)) {
        sprite_.setTexture(texture_);
        has_texture_ = true;
    } else {
        has_texture_ = false;
    }
    return this;
}

ActionPanel* ActionPanel::CreateInstance() {
    ActionPanel* instance = new ActionPanel;
    instance -> pos_ = ACTION_PANEL_POS;
    instance -> panel_shape_
            .setFillColor(instance -> panel_color_);
    instance -> panel_shape_
            .setPosition(instance -> pos_.x, instance -> pos_.y);
    return instance;
}

ActionPanel::ActionPanel()
    : panel_shape_(ACTION_PANEL_SIZE),
      panel_color_(sf::Color(201, 214, 213, 255)),
      components_() {}

void ActionPanel::Click() {
    LOG(INFO) << "ActionPanel clicked";
}

bool ActionPanel::OnMouse(sf::Vector2i cursor) const {
    return panel_shape_
            .getGlobalBounds()
            .contains((float) cursor.x, (float) cursor.y);
}

void ActionPanel::Draw(sf::RenderWindow* window) {
    window -> draw(panel_shape_);
    for (size_t i = 0; i < components_.size(); ++i)
        components_[i] -> Draw(window);
}

void ActionPanel::AddComponent(MapObject* pcomponent) {
    components_.push_back(pcomponent);
}


PlayerCard* PlayerCard::CreateInstance(sf::Vector2f pos, Player& player) {
    PlayerCard* instance = new PlayerCard(PLAYER_CARD_SIZE, player);
    instance -> pos_ = pos;
    instance -> shape_.setPosition(pos);

    instance -> playerpic_shape_.setPosition(pos + sf::Vector2f(10, 10));
    sf::Color player_color;
    switch (player.get_id()) {
        case 0:
            player_color = sf::Color::Red;
            break;
        case 1:
            player_color = sf::Color::Cyan;
            break;
        case 2:
            player_color = sf::Color::Magenta;
            break;
        default:
            player_color = sf::Color::White;
            break;
    }
    instance -> playerpic_shape_.setFillColor(player_color);

    if (!instance -> font_.loadFromFile("/Library/Fonts/Arial Black.ttf")) {
        return nullptr;
    }
    instance -> name_text_.setPosition(pos + sf::Vector2f(75, 10));
    instance -> name_text_.setCharacterSize(20);
    instance -> name_text_.setFont(instance -> font_);
    instance -> name_text_.setString(std::to_string(player.get_id()));
    instance -> name_text_.setColor(sf::Color::Black);

    instance -> resources_text_.setPosition(pos +
            sf::Vector2f(115, 5));
    instance -> resources_text_.setCharacterSize(12);
    instance -> resources_text_.setFont(instance -> font_);
    instance -> resources_text_.setString("0");
    instance -> resources_text_.setColor(sf::Color::Black);
    return instance;
}

PlayerCard::PlayerCard(sf::Vector2f sz, Player& p)
    : Button(sz),
      player_(p),
      playerpic_shape_(sf::Vector2f(60, 60)),
      font_() {}

void PlayerCard::Draw(sf::RenderWindow* window) {
    int* res = player_.get_resources();
    resources_text_.setString(
            "bricks " + std::to_string(res[0]) + ",\n" +
            "wool " + std::to_string(res[1]) + ",\n" +
            "ore " + std::to_string(res[2]) + ",\n" +
            "grain " + std::to_string(res[3]) + ",\n" +
            "lumber " + std::to_string(res[4])
        );

    Button::Draw(window);
    window -> draw(playerpic_shape_);
    window -> draw(name_text_);
    window -> draw(resources_text_);
}

void PlayerCard::GainFocus() {
    idle_ += color_active_delta_;
}

void PlayerCard::LoseFocus() {
    idle_ -= color_active_delta_;
}

PlayerPanel* PlayerPanel::CreateInstance(int* lpc) {
    PlayerPanel* instance = new PlayerPanel;
    instance -> pos_ = PLAYER_PANEL_POS;
    instance -> panel_shape_
            .setFillColor(instance -> panel_color_);
    instance -> panel_shape_
            .setPosition(instance -> pos_.x, instance -> pos_.y);
    instance -> lpc_ = lpc;
    return instance;
}

PlayerPanel::PlayerPanel()
    : panel_shape_(PLAYER_PANEL_SIZE),
      panel_color_(sf::Color::Green),
      player_cards_() {}

void PlayerPanel::Click() {
    LOG(INFO) << "PlayerPanel clicked";
    sf::Vector2i cursor = sf::Mouse::getPosition(*window_);
    for (size_t i = 0; i < player_cards_.size(); ++i) {
        if (player_cards_[i] -> OnMouse(cursor)) {
            player_cards_[i] -> Click();
            *lpc_ = i;
            LOG(INFO) << "lpc " << *lpc_;
        }
    }

}

bool PlayerPanel::OnMouse(sf::Vector2i cursor) const {
    return panel_shape_
            .getGlobalBounds()
            .contains((float) cursor.x, (float) cursor.y);
}

void PlayerPanel::Draw(sf::RenderWindow* window) {
    window -> draw(panel_shape_);
    for (size_t i = 0; i < player_cards_.size(); ++i)
        player_cards_[i] -> Draw(window);
    window_ = window;
}

void PlayerPanel::Insert(Player& player) {
    PlayerCard* pc = (PlayerCard*)
        PlayerCard::CreateInstance(pos_ +
                sf::Vector2f(10, 10 + player_cards_.size() *
                    (PLAYER_CARD_SIZE.y + 20)), player)
                -> SetColors(sf::Color(155, 155, 255, 240),
                             sf::Color(255, 255, 255, 255))
                -> AddCallback(
                        [this] () {
                            LOG(INFO) << "PlayerCard clicked";
                        });

    player_cards_.push_back(pc);
}

void PlayerPanel::SetIdle(int index) {
    dynamic_cast<PlayerCard*>(player_cards_[index]) ->
        LoseFocus();
}

void PlayerPanel::SetActive(int index) {
    if (last_active_id_ != -1)
        SetIdle(last_active_id_);
    last_active_id_ = index;
    dynamic_cast<PlayerCard*>(player_cards_[index]) ->
        GainFocus();
}

Map::Map(sf::RenderWindow* window)
    : Map(new Hex, window) {}

Map::Map(Hex* root, sf::RenderWindow* window)
    : root_(root),
      window_(window),
      notifications_(NotificationArea::
              CreateInstance("/Library/Fonts/Arial Black.ttf")),
      action_panel_(ActionPanel::CreateInstance()),
      player_panel_(PlayerPanel::CreateInstance(&last_player_clicked_)),
      hexagon_(sf::CircleShape(HEX_SIZE - HEX_OUTLINE_SIZE,
              HEX_PRECISION)),
      hex_text_(),
      hex_font_(),
      sea_texture_(),
      desert_texture_(),
      hills_texture_(),
      pasture_texture_(),
      mountains_texture_(),
      fields_texture_(),
      village_texture_(),
      road_texture_(),
      sea_sprite_(),
      desert_sprite_(),
      hills_sprite_(),
      pasture_sprite_(),
      mountains_sprite_(),
      fields_sprite_(),
      village_sprite_(),
      road_sprite_(),
      point_circle_(sf::CircleShape(POINT_SIZE - POINT_OUTLINE_SIZE,
              POINT_PRECISION)),
      line_array_(sf::VertexArray(sf::Lines, 2)) {
    Init();
}

Map::~Map() {
    map_objects_.clear();
    points_.clear();
    lines_.clear();

    delete root_;
    delete notifications_;
    delete action_panel_;
    delete player_panel_;
}

void Map::Init() {
    hexagon_.setOutlineThickness(HEX_OUTLINE_SIZE);
    hexagon_.setOutlineColor(sf::Color::Black);
    if (!sea_texture_.loadFromFile("hexes_sea.png")) {
        LOG(ERROR) << "Can't load sea texture";
    } else {
        sea_sprite_.setTexture(sea_texture_);
    }

    if (!desert_texture_.loadFromFile("hexes_desert.png")) {
        LOG(ERROR) << "Can't load desert texture";
    } else {
        desert_sprite_.setTexture(desert_texture_);
    }

    if (!hills_texture_.loadFromFile("hexes_hills.png")) {
        LOG(ERROR) << "Can't load hills texture";
    } else {
        hills_sprite_.setTexture(hills_texture_);
    }

    if (!pasture_texture_.loadFromFile("hexes_pasture.png")) {
        LOG(ERROR) << "Can't load pasture texture";
    } else {
        pasture_sprite_.setTexture(pasture_texture_);
    }

    if (!mountains_texture_.loadFromFile("hexes_mountains.png")) {
        LOG(ERROR) << "Can't load mountains texture";
    } else {
        mountains_sprite_.setTexture(mountains_texture_);
    }

    if (!fields_texture_.loadFromFile("hexes_fields.png")) {
        LOG(ERROR) << "Can't load fields texture";
    } else {
        fields_sprite_.setTexture(fields_texture_);
    }

    if (!forest_texture_.loadFromFile("hexes_forest.png")) {
        LOG(ERROR) << "Can't load forest texture";
    } else {
        forest_sprite_.setTexture(forest_texture_);
    }

    if (!village_texture_.loadFromFile("village.png")) {
        LOG(ERROR) << "Can't load village texture";
    } else {
        village_sprite_.setTexture(village_texture_);
        village_sprite_.scale(sf::Vector2f(0.25, 0.25));
    }

    if (!road_texture_.loadFromFile("road_tile.png")) {
        LOG(ERROR) << "Can't load road texture";
    } else {
        road_texture_.setRepeated(true);
        road_sprite_.setTexture(road_texture_);
    }

    point_circle_.setFillColor(sf::Color::Red);
    point_circle_.setOutlineColor(sf::Color::Black);
    point_circle_.setOutlineThickness(POINT_OUTLINE_SIZE);

    hex_text_.setCharacterSize(HEX_SIZE / 2);
    hex_text_.setColor(sf::Color::Black);
    if (!hex_font_.loadFromFile("/Library/Fonts/Arial Black.ttf")) {
        LOG(ERROR) << "Error loading font";
        return;
    } else {
        hex_text_.setFont(hex_font_);
    }
}

void Map::Generate() {
    srand(time(NULL));
    LOG(INFO) << "Generating map...";
    int v_dims[] = {2, 5, 8, 11, 11, 11, 11};
    double h_dims[] = {
        cos(30.0 * M_PI / 180.0),
        cos(30.0 * M_PI / 180.0),
        cos(30.0 * M_PI / 180.0),
        2 * cos(30.0 * M_PI / 180.0),
        2 * cos(30.0 * M_PI / 180.0),
        2 * cos(30.0 * M_PI / 180.0)
    };

    int nums_arr[] = {7, 8, 5, 4, 3, 10, 2, 11,
        6, 11, 9, 6, 12, 5, 4, 3, 9, 10, 8};
    int nums_ind = 0;

    int wx = window_ -> getSize().x;
    int wy = window_ -> getSize().y;
    int deltax = HEX_SIZE * cos(30.0 * M_PI / 180.0);
    int deltay = HEX_SIZE * (1 + sin(30.0 * M_PI / 180.0));

    std::vector<int> types;
    for (int i = TYPE_SEA; i <= TYPE_FOREST; ++i)
        for (int j = 0; j < TYPE_COUNT[i]; ++j)
            types.push_back(i);

//    std::vector<int> nums(&NUMS_ARR[0], &NUMS_ARR[0]
//            + (HEXES_NUM - 1));

    int m = HEXES_NUM - 1;
    int k = types.size();

    Hex* curr = root_;

    const int horizontal_start = (wx - HEX_SIZE * 2 *
            (2 + GRID_SIZE) * cos(30.0 * M_PI / 180.0)) / 2;
    int horizontal_offset = horizontal_start;

    for (int i = 0; i < GRID_SIZE + 2; ++i) {
        int vertical_offset = (wy - v_dims[i] * HEX_SIZE) / 2;
        horizontal_offset = horizontal_start;
        for (int j = 0; j < i; ++j)
            horizontal_offset += HEX_SIZE * h_dims[j];
        for (int j = 0; j < dims_[i]; ++j) {
            if (NULL == curr) {
                LOG(ERROR) << "NULL == curr";
                return;
            }

            curr -> set_pos(sf::Vector2f(horizontal_offset,
                        vertical_offset));
            if (i > 0 && j > 0) {
                Hex* left = curr -> up_left -> down_left;
                curr -> left = left;
                left -> right = curr;
                if (NULL != left -> down_right) {
                    curr -> down_left = left -> down_right;
                    left -> down_right -> up_right = curr;
                }
            }

            int type;
            if ((i > 0 && i < GRID_SIZE + 1) &&
                    (j > 0 && j < dims_[i] - 1)) {
                if (1 == i && 1 == j) {
                    type = TYPE_DESERT;
                } else {
                    int type_ind = rand() % k;
                    type = types[type_ind];
                    types.erase(types.begin() + type_ind);
                    --k;
                }
            } else {
                type = TYPE_SEA;
            }

            curr -> set_type(type);
            if (TYPE_SEA == type) {
                curr -> set_num(0);
            } else {
                //int neighbor_sum = 0;
                //int neighbor_count = 0;
                //if (NULL != curr -> up_left) {
                //    neighbor_count++;
                //    neighbor_sum += curr -> up_left -> get_num();
                //}
                //if (NULL != curr -> left) {
                //    neighbor_count++;
                //    neighbor_sum += curr -> left -> get_num();
                //}
                //if (NULL != curr -> down_left) {
                //    neighbor_count++;
                //    neighbor_sum += curr -> down_left -> get_num();
                //}

                //int num_ind;
                //if (neighbor_count == 0) {
                //    num_ind = rand() % m;
                //} else {
                //    num_ind = 0;
                //    int desired = 6 + neighbor_count * 6 -
                //        neighbor_sum;
                //    int minabs = 6;
                //    for (int ind = 0; ind < m; ++ind) {
                //        int currabs = abs(desired - nums[ind]);
                //        if (minabs > currabs) {
                //            minabs = currabs;
                //            num_ind = ind;
                //        }
                //    }
                //}
                //curr -> set_num(nums[num_ind]);
                curr -> set_num(nums_arr[nums_ind++]);
                //nums.erase(nums.begin() + num_ind);
                --m;
            }
            hexes_.push_back(curr);
            hexes_by_num_[curr -> get_num()].push_back(curr);
            map_objects_.push_back(curr);
            if (j < dims_[i] - 1 && NULL == curr -> down_right) {
                curr -> down_right = new Hex;
                curr -> down_right -> up_left = curr;
                curr = curr -> down_right;
            }
            vertical_offset += deltay;
            horizontal_offset += deltax;
        }
        for (int j = 1; j < dims_[i]; ++j) {
            curr = curr -> up_left;
        }
        if (i < (GRID_SIZE + 2) / 2) {
            curr -> up_right = new Hex;
            curr -> up_right -> down_left = curr;

            curr = curr -> up_right;
        } else if (i < (GRID_SIZE + 2) - 1) {
            curr -> right = new Hex;
            curr -> right -> left = curr;

            curr = curr -> right;
            curr -> down_left = curr -> left -> down_right;
            curr -> down_left -> up_right = curr;
        }
    }

    GeneratePoints();
    GenerateLines();
    //GenerateActionPanel();
    map_objects_.push_back(notifications_);
    map_objects_.push_back(action_panel_);
    map_objects_.push_back(player_panel_);
    LOG(INFO) << "Map generated successfully";
}

void Map::GeneratePoints() {
    int deltax = HEX_SIZE * cos(30.0 * M_PI / 180.0);
    int deltay = HEX_SIZE * (1 + sin(30.0 * M_PI / 180.0));
    double sqrt2 = sqrt(2.0);
    sf::Vector2f to_center = sf::Vector2f(HEX_SIZE * sqrt2 * .5,
                         HEX_SIZE * sqrt2 * .5);
    for (size_t i = 0; i < hexes_.size(); ++i) {
        Hex* curr = hexes_[i];
        Hex* ul = curr -> up_left;
        Hex* ur = curr -> up_right;
        Hex* r = curr -> right;

        sf::Vector2f curr_pos = curr -> get_pos();
        if (ul && ur) {
            Point* new_point = new Point(ul, ur, curr);
            new_point -> set_pos(
                    curr_pos + sf::Vector2f((float) deltax -
                        .5f * HEX_OUTLINE_SIZE,
                        -POINT_SIZE));
            AddPoint(new_point);
        }
        if (ur && r) {
            Point* new_point = new Point(ur, r, curr);
            new_point -> set_pos(
                    curr_pos + sf::Vector2f((float) 2 * deltax -
                        .5f * HEX_OUTLINE_SIZE,
                        cos(60.0 * M_PI / 180.0) * HEX_SIZE -
                        POINT_SIZE - HEX_OUTLINE_SIZE));
            AddPoint(new_point);
        }
    }
}

void Map::GenerateLines() {
    for (size_t i = 0; i < points_.size() - 1; ++i) {
        for (size_t j = i + 1; j < points_.size(); ++j) {
            Line* line = Line::FromPoints(points_[i], points_[j]);
            if (line) {
                line -> set_pos(points_[i] -> get_pos());
                AddLine(line);
                points_[i] -> AddLine(line);
                points_[j] -> AddLine(line);
            }
        }
    }
}

/**
 * Deprecated
 */
//void Map::GenerateActionPanel() {
//    Button* p_new_village_btn =
//        Button::CreateInstance(ACTION_PANEL_POS +
//            sf::Vector2f(45, 10), sf::Vector2f(30, 30))
//                -> SetColors(sf::Color(196, 53, 52, 196),
//                             sf::Color(196, 53, 52, 255));
//    action_panel_ -> AddComponent(p_new_village_btn);
//    map_objects_.push_back(p_new_village_btn);
//
//    Button* p_dice_btn =
//        Button::CreateInstance(ACTION_PANEL_POS +
//            sf::Vector2f(10, 10), sf::Vector2f(30, 30))
//                -> SetColors(sf::Color(53, 196, 72, 196),
//                             sf::Color(53, 196, 72, 255));
//    action_panel_ -> AddComponent(p_dice_btn);
//    map_objects_.push_back(p_dice_btn);
//}

void Map::DrawPoints() const {
//    TODO : remove stub
    for (size_t i = 0; i < points_.size(); ++i) {
        Point* curr = points_[i];
//        point_circle_.setPosition(curr -> get_pos());
        village_sprite_.setPosition(curr -> get_pos() -
                sf::Vector2f(4, 4));
        sf::Color fill_color;
        switch (curr -> get_owner_id()) {
            case 0:
                fill_color = sf::Color::Red;
                break;
            case 1:
                fill_color = sf::Color::Cyan;
                break;
            case 2:
                fill_color = sf::Color::Magenta;
                break;
            case 3:
                fill_color = sf::Color::White;
                break;
            default:
                fill_color = sf::Color(255, 255, 255, 20);
                break;
        }
//        point_circle_.setFillColor(fill_color);
        village_sprite_.setColor(fill_color);
//        window_ -> draw(point_circle_);
        window_ -> draw(village_sprite_);
    }
}

void Map::DrawLines() const {
    Point* points[2];
    sf::Vector2f positions[2];
    for (size_t i = 0; i < lines_.size(); ++i) {
        Line* curr = lines_[i];
//        line_rectangle_.setPosition(curr -> get_pos());
//        line_rectangle_.setRotation(curr -> get_rotation());
//        window_ -> draw(line_rectangle_);
        curr -> get_points(points);
        double sqrt2 = sqrt(2.0);
        sf::Vector2f to_center = sf::Vector2f(POINT_SIZE * sqrt2 * .5,
                         POINT_SIZE * sqrt2 * .5);
        sf::Color fill_color;
        switch (curr -> get_owner_id()) {
            case 0:
                fill_color = sf::Color::Red;
                break;
            case 1:
                fill_color = sf::Color::Cyan;
                break;
            case 2:
                fill_color = sf::Color::Magenta;
                break;
            case 3:
                fill_color = sf::Color::White;
                break;
            default:
                fill_color = sf::Color(255, 255, 255, 20);
                break;
        }
        auto pos0 = points[0] -> get_pos();
        auto pos1 = points[1] -> get_pos();
//        line_array_[0].position = pos0 + to_center;
//        line_array_[0].color = fill_color;
//        line_array_[1].position = pos1 + to_center;
//        line_array_[1].color = fill_color;
//        window_ -> draw(line_array_);

        auto road_dim = sf::Vector2i {
            std::max(abs(pos0.y - pos1.y),
                    abs(pos0.x - pos1.x)), 10
        };
        int dx = pos0.x - pos1.x;
        int dy = pos0.y - pos1.y;
        int rotation = 0;
        if (!dx) {
            if (dy > 0)
                rotation = -90;
            else
                rotation = 90;
        } else if (dx > 0) {
            if (dy > 0)
                rotation = -150;
            else
                rotation = 150;
        } else {
            if (dy > 0)
                rotation = -30;
            else
                rotation = 30;
        }
        road_sprite_.setRotation(rotation);
        road_sprite_.setPosition(pos0 + to_center);
        road_sprite_.setTextureRect({ 0, 0,
                road_dim.x, road_dim.y });
        road_sprite_.setColor(fill_color);
        window_ -> draw(road_sprite_);
    }
}

void Map::DrawMap() const {
    sf::Vector2i point = sf::Mouse::getPosition(*window_);

    int dims_[] = {GRID_SIZE - 1, GRID_SIZE, GRID_SIZE + 1,
        GRID_SIZE + 2, GRID_SIZE + 1, GRID_SIZE, GRID_SIZE - 1};
    int k = 0;
    Hex* curr = hexes_[k++];
//    sf::CircleShape pos_circle(10, 20);
//    pos_circle.setFillColor(sf::Color::Magenta);
//    sf::CircleShape center_circle(10, 20);
//    center_circle.setFillColor(sf::Color::Cyan);
    for (int i = 0; i < GRID_SIZE + 2; ++i) {
        for (int j = 0; j < dims_[i]; ++j) {
            if (0 == curr -> get_num())
                hexagon_.setFillColor(sf::Color::Yellow);
            else
                hexagon_.setFillColor(sf::Color::White);

            sf::Vector2f curr_pos = curr -> get_pos();
            hexagon_.setPosition(curr_pos);
            sf::Sprite* curr_sprite;
            int curr_type = curr -> get_type();
            switch (curr_type) {
                case TYPE_SEA:
                    curr_sprite = &sea_sprite_;
                    break;
                case TYPE_DESERT:
                    curr_sprite = &desert_sprite_;
                    break;
                case TYPE_HILLS:
                    curr_sprite = &hills_sprite_;
                    break;
                case TYPE_PASTURE:
                    curr_sprite = &pasture_sprite_;
                    break;
                case TYPE_MOUNTAINS:
                    curr_sprite = &mountains_sprite_;
                    break;
                case TYPE_FIELDS:
                    curr_sprite = &fields_sprite_;
                    break;
                case TYPE_FOREST:
                    curr_sprite = &forest_sprite_;
                    break;
                default:
                    break;
            }
            curr_sprite -> setPosition(curr_pos);
            if (curr -> OnMouse(point)) {
//                hexagon_.setFillColor(TYPE_COLOR[curr_type] +
//                        sf::Color(0, 0, 0, 75));
                curr_sprite -> setColor(
                        sf::Color(255, 255, 255, 255));
            } else {
//                hexagon_.setFillColor(TYPE_COLOR[curr_type]);
                curr_sprite -> setColor(
                        sf::Color(255, 255, 255, 205));
            }
//            window_ -> draw(hexagon_);
            window_ -> draw(*curr_sprite);

//            pos_circle.setPosition(curr_pos);
//            window_ -> draw(pos_circle);
//            center_circle.setPosition(curr_pos + sf::Vector2f(HEX_SIZE * sqrt(2.0) * .5,
//                         HEX_SIZE * sqrt(2.0) * .5));
//            window_ -> draw(center_circle);

            if (curr_type != 0) {
                hex_text_.setPosition(curr_pos +
                        sf::Vector2f(.7f * HEX_SIZE, .7f * HEX_SIZE));
                hex_text_.setString(std::to_string(curr -> get_num()));
                window_ -> draw(hex_text_);
            }
            curr = hexes_[k++];
        }
    }
}

void Map::Draw() const {
    DrawMap();
    DrawLines();
    DrawPoints();
    notifications_ -> Draw(window_);
    notifications_ -> Update();

    action_panel_ -> Draw(window_);
    player_panel_ -> Draw(window_);
}

void Map::Click(Player* requester) {
    last_requester_ = requester;
    sf::Vector2i point = sf::Mouse::getPosition(*window_);
    for (size_t i = 0; i < map_objects_.size(); ++i)
        if (map_objects_[i] -> OnMouse(point))
            map_objects_[i] -> Click();
}

void Map::ShowNotification(std::string text) {
    notifications_ -> MakeVisible();
    notifications_ -> SetContent(text);
}

void Map::ShowNotification(std::string text, int duration) {
    notifications_ -> MakeVisible(duration);
    notifications_ -> SetContent(text);
}

Point* Map::AddVillage(Player* player) {
    sf::Vector2i cursor = sf::Mouse::getPosition(*window_);
    int player_id = player -> get_id();
    for (size_t i = 0; i < points_.size(); ++i) {
        Point* point = points_[i];
        if (point -> OnMouse(cursor) &&
                point -> CanBeOwned(player_id) &&
                TryAddPoint(point)) {
            point -> set_owner_id(player_id);
            return point;
        }
    }
    return NULL;
}

bool Map::AddVillage() {
    return NULL != AddVillage(last_requester_);
}

Line* Map::AddRoad(Player* player) {
    sf::Vector2i cursor = sf::Mouse::getPosition(*window_);
    int player_id = player -> get_id();
    for (size_t i = 0; i < lines_.size(); ++i) {
        Line* line = lines_[i];
        if (line -> OnMouse(cursor) &&
                line -> CheckOwnership(player_id) &&
                -1 == line -> get_owner_id()) {
            line -> set_owner_id(player_id);
            return line;
        }
    }
    return NULL;
}

void Map::AddButton(Button* b) {
    action_panel_ -> AddComponent(b);
    map_objects_.push_back(b);
}

std::vector<Triple<int, int, int> >
        Map::GenerateResources(int num) {
    //DO STH
    LOG(INFO) << "GenerateResources of " + std::to_string(num) +
        " is called";
    std::vector<Triple<int, int, int> > result;

    std::vector<Hex*> matched_hexes = hexes_by_num_[num];
    for (size_t i = 0; i < matched_hexes.size(); ++i) {
        std::vector<int> players_ids =
                matched_hexes[i] -> GetBoundPlayersIds();
        for (size_t j = 0; j < players_ids.size(); ++j) {
            result.push_back(Triple<int, int, int>::make_triple(
                        players_ids[j],
                        matched_hexes[i] -> get_type(),
                        1)); //TODO: add support for cities
        }
    }
    return result;
}

void Map::AddRandomVillageRoad(Player* curr) {
    bool added = false;
    int player_id = curr -> get_id();
    while (!added) {
        Point* point = points_[rand() % points_.size()];
        if (-1 == point -> get_owner_id() &&
                TryAddPoint(point)) {
            point -> set_owner_id(player_id);
            added = true;
        }
    }

    added = false;
    while (!added) {
        Line* line = lines_[rand() % lines_.size()];
        if (line -> CheckOwnership(player_id) &&
                -1 == line -> get_owner_id()) {
            line -> set_owner_id(player_id);
            added = true;
        }
    }
}

void Map::DisplayPlayersInfo(std::vector<Player*> players) {
    for (size_t i = 0; i < players.size(); ++i) {
        player_panel_ -> Insert(*players[i]);
    }
}

void Map::SetActivePlayer(int _id) {
    player_panel_ -> SetActive(_id);
}

int Map::get_lpc() const {
    return last_player_clicked_;
}

Point* Map::AddPoint(Point* point) {
    points_.push_back(point);
    map_objects_.push_back(point);
    return point;
}

Line* Map::AddLine(Line* line) {
    lines_.push_back(line);
    map_objects_.push_back(line);
    return line;
}

bool Map::TryAddPoint(Point* point) {
    Hex* point_hexes[3];
    point -> get_hexes(point_hexes);
    bool res = true;
    for (int i = 0; i < 3 && res; ++i)
        res = point_hexes[i] -> CanBeAdded(point);
    if (res) {
        for (int i = 0; i < 3; ++i)
            point_hexes[i] -> AddPoint(point);
    }
    return res;
}
