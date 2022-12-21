#pragma once

#include <imgui.h>
#include <memory>
#include <vector>
#include <functional>

namespace hl
{
    namespace easygui
    {
        class Object
        {
        public:
            Object() = default;
            virtual ~Object() = default;

            virtual void update() = 0;
        };

        using GuiObject = std::shared_ptr<Object>;
        using GuiObjectPtr = Object *;

        class Window
        {
        private:
            std::vector<GuiObject> m_objects;
            std::string m_name;
            bool m_open = true;
            ImGuiWindowFlags m_flags = 0;

        public:
            Window(const std::string& name, bool open = true, ImGuiWindowFlags flags = 0)
                : m_name(name), m_open(open), m_flags(flags)
            {
            }

            ~Window() = default;

            void update()
            {
                ImGui::Begin(m_name.c_str(), &m_open, m_flags);
                for (auto& object : m_objects)
                {
                    object->update();
                }
                ImGui::End();
            }

            Window& add_object(GuiObject&& object)
            {
                m_objects.push_back(std::move(object));
                return *this;
            }

            Window& add_object(GuiObjectPtr object)
            {
                m_objects.push_back(GuiObject(object));
                return *this;
            }
        };

        using GuiWindow = std::shared_ptr<Window>;
        using GuiWindowPtr = Window *;

        class MenuItem : public Object
        {
        private:
            std::string m_name;
            std::string m_shortcut = "";
            std::function<void()> m_callback = nullptr;

        public:
            MenuItem(const std::string& name, const std::string& shortcut, std::function<void()> callback)
                : m_name(name), m_shortcut(shortcut), m_callback(callback)
            {
            }

            virtual void update() override
            {
                if (ImGui::MenuItem(m_name.c_str(), m_shortcut.c_str()))
                    m_callback();
            }

            bool is_valid() const
            {
                return !m_name.empty() && m_callback != nullptr;
            }

            MenuItem& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            MenuItem& set_shortcut(const std::string& shortcut)
            {
                m_shortcut = shortcut;
                return *this;
            }

            MenuItem& set_callback(std::function<void()> callback)
            {
                m_callback = callback;
                return *this;
            }
        };

        using GuiMenuItem = std::shared_ptr<MenuItem>;
        using GuiMenuItemPtr = MenuItem *;

        class Menu : public Object
        {
        private:
            std::string m_name;
            std::vector<GuiMenuItem> m_items;
        
        public:
            Menu(const std::string& name)
                : m_name(name)
            {
            }

            virtual void update() override
            {
                if (ImGui::BeginMenu(m_name.c_str()))
                {
                    for (auto& item : m_items)
                    {
                        item->update();
                    }

                    ImGui::EndMenu();
                }
            }

            void add_item(const GuiMenuItem& item)
            {
                m_items.push_back(item);
            }

            void add_item(GuiMenuItemPtr item)
            {
                m_items.push_back(GuiMenuItem(item));
            }
        };

        using GuiMenu = std::shared_ptr<Menu>;
        using GuiMenuPtr = Menu *;

        class MenuBar : public Object
        {
        private:
            std::vector<GuiMenu> m_menus;

        public:
            MenuBar() = default;
            virtual ~MenuBar() = default;

            virtual void update() override
            {
                if (ImGui::BeginMenuBar())
                {
                    for (auto& menu : m_menus)
                    {
                        menu->update();
                    }

                    ImGui::EndMenuBar();
                }
            }

            void add_menu(const GuiMenu& menu)
            {
                m_menus.push_back(menu);
            }

            void add_menu(GuiMenuPtr menu)
            {
                m_menus.push_back(GuiMenu(menu));
            }
        };

        using GuiMenuBar = std::shared_ptr<MenuBar>;
        using GuiMenuBarPtr = MenuBar *;

        class ColorEdit : public Object
        {
        private:
            std::string m_name;
            ImVec4 *m_color;
            
        
        public:
            ColorEdit(const std::string& name, ImVec4 *color)
                : m_name(name), m_color(color)
            {
            }

            virtual void update() override
            {
                ImGui::ColorEdit4(m_name.c_str(), &m_color->x);
            }

            ColorEdit& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            ColorEdit& set_color(ImVec4 *color)
            {
                m_color = color;
                return *this;
            }
        };

        using GuiColorEdit = std::shared_ptr<ColorEdit>;
        using GuiColorEditPtr = ColorEdit *;

        class PlotLines : public Object
        {
        private:
            std::string m_name;
            std::vector<float> m_values;
            int m_values_offset = 0;
            std::string m_overlay_text = "";
            float m_scale_min = FLT_MAX;
            float m_scale_max = FLT_MAX;
            ImVec2 m_graph_size = ImVec2(0, 0);
            int m_stride = sizeof(float);
        
        public:
            PlotLines(const std::string& name, const std::vector<float>& values)
                : m_name(name), m_values(values)
            {
            }

            virtual void update() override
            {
                ImGui::PlotLines(m_name.c_str(), &m_values[0], m_values.size(), m_values_offset, m_overlay_text.c_str(), m_scale_min, m_scale_max, m_graph_size, m_stride);
            }

            PlotLines& set_values_offset(int values_offset)
            {
                m_values_offset = values_offset;
                return *this;
            }

            PlotLines& set_overlay_text(const std::string& overlay_text)
            {
                m_overlay_text = overlay_text;
                return *this;
            }

            PlotLines& set_scale_min(float scale_min)
            {
                m_scale_min = scale_min;
                return *this;
            }

            PlotLines& set_scale_max(float scale_max)
            {
                m_scale_max = scale_max;
                return *this;
            }

            PlotLines& set_graph_size(const ImVec2& graph_size)
            {
                m_graph_size = graph_size;
                return *this;
            }

            PlotLines& set_stride(int stride)
            {
                m_stride = stride;
                return *this;
            }
        };

        using GuiPlotLines = std::shared_ptr<PlotLines>;
        using GuiPlotLinesPtr = PlotLines *;

        class Histogram : public Object
        {
        private:
            std::string m_name;
            std::vector<float> m_values;
            int m_values_offset = 0;
            std::string m_overlay_text = "";
            float m_scale_min = FLT_MAX;
            float m_scale_max = FLT_MAX;
            ImVec2 m_graph_size = ImVec2(0, 0);
            int m_stride = sizeof(float);

        public:

            Histogram(const std::string& name, const std::vector<float>& values)
                : m_name(name), m_values(values)
            {
            }

            virtual void update() override
            {
                ImGui::PlotHistogram(m_name.c_str(), &m_values[0], m_values.size(), m_values_offset, m_overlay_text.c_str(), m_scale_min, m_scale_max, m_graph_size, m_stride);
            }

            Histogram& set_values_offset(int values_offset)
            {
                m_values_offset = values_offset;
                return *this;
            }

            Histogram& set_overlay_text(const std::string& overlay_text)
            {
                m_overlay_text = overlay_text;
                return *this;
            }

            Histogram& set_scale_min(float scale_min)
            {
                m_scale_min = scale_min;
                return *this;
            }

            Histogram& set_scale_max(float scale_max)
            {
                m_scale_max = scale_max;
                return *this;
            }

            Histogram& set_graph_size(const ImVec2& graph_size)
            {
                m_graph_size = graph_size;
                return *this;
            }

            Histogram& set_stride(int stride)
            {
                m_stride = stride;
                return *this;
            }
        };

        class Text : public Object
        {
        private:
            std::string *m_text;
            float rgba[4];
        
        public:
            Text(std::string *text, float r, float g, float b, float a)
                : m_text(text)
            {
                set_color(r, g, b, a);
            }

            virtual void update() override
            {
                ImGui::TextUnformatted(m_text->c_str());
            }

            Text& set_text(std::string* text)
            {
                m_text = text;
                return *this;
            }

            void set_color(float r, float g, float b, float a)
            {
                rgba[0] = r;
                rgba[1] = g;
                rgba[2] = b;
                rgba[3] = a;
            }

            float *get_color()
            {
                return rgba;
            }
        };

        using GuiText = std::shared_ptr<Text>;
        using GuiTextPtr = Text *;

        class Logger : public Object
        {
        private:
            size_t m_max_lines = 0;
            std::vector<GuiText> m_lines;
            float rgba[4];

        public:
            Logger(float r, float g, float b, float a)
            {
                set_color(r, g, b, a);
            }

            Logger& set_color(float r, float g, float b, float a)
            {
                rgba[0] = r;
                rgba[1] = g;
                rgba[2] = b;
                rgba[3] = a;
                for (auto& line : m_lines)
                {
                    line->set_color(r, g, b, a);
                }
                return *this;
            }

            Logger& set_max_lines(size_t max_lines)
            {
                m_max_lines = max_lines;
                return *this;
            }

            Logger& add_text(const std::string& text)
            {
                auto stext = std::make_shared<Text>(new std::string(text), rgba[0], rgba[1], rgba[2], rgba[3]);
                m_lines.push_back(stext);
                if (m_max_lines != 0 && m_lines.size() > m_max_lines)
                {
                    m_lines.erase(m_lines.begin());
                }
                return *this;
            }

            virtual void update() override
            {
                for (auto& line : m_lines)
                {
                    line->update();
                }
            }
        };
        
        using GuiLogger = std::shared_ptr<Logger>;
        using GuiLoggerPtr = Logger *;

        class Button : public Object
        {
        private:
            std::string m_name;
            std::function<void()> m_callback;

        public:
            Button(const std::string& name, const std::function<void()>& callback)
                : m_name(name), m_callback(callback)
            {
            }

            virtual void update() override
            {
                if (ImGui::Button(m_name.c_str()))
                {
                    m_callback();
                }
            }

            Button& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            Button& set_callback(const std::function<void()>& callback)
            {
                m_callback = callback;
                return *this;
            }

            Button& set_callback(std::function<void()>&& callback)
            {
                m_callback = std::move(callback);
                return *this;
            }
        };

        using GuiButton = std::shared_ptr<Button>;
        using GuiButtonPtr = Button *;

        template <typename T>
        class SliderT : public Object
        {
        private:
            std::string m_name;
            T *m_value;
            T m_min;
            T m_max;
            std::string m_format;
            T m_power = 1.0f;
        
        public:
            SliderT(const std::string& name, T *value, T min, T max)
                : m_name(name), m_value(value), m_min(min), m_max(max)
            {
                if constexpr(std::is_same_v<T, float>)
                {
                    m_format = "%.3f";
                }
                else
                {
                    m_format = "%d";
                }
            }

            virtual void update() override
            {
                if constexpr(std::is_same_v<T, float>)
                {
                    ImGui::SliderFloat(m_name.c_str(), m_value, m_min, m_max, m_format.c_str(), m_power);
                }
                else
                {
                    ImGui::SliderInt(m_name.c_str(), m_value, m_min, m_max, m_format.c_str(), m_power);
                }
            }

            SliderT& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            SliderT& set_value(const std::vector<T>& value)
            {
                m_value = value;
                return *this;
            }

            SliderT& set_min(T min)
            {
                m_min = min;
                return *this;
            }

            SliderT& set_max(SliderT max)
            {
                m_max = max;
                return *this;
            }

            SliderT& set_format(const std::string& format)
            {
                m_format = format;
                return *this;
            }

            SliderT& set_power(T power)
            {
                m_power = power;
                return *this;
            }
        };

        using SliderFloat = SliderT<float>;
        using SliderInt = SliderT<int>;

        using GuiSliderFloat = std::shared_ptr<SliderFloat>;
        using GuiSliderFloatPtr = SliderFloat *;

        using GuiSliderInt = std::shared_ptr<SliderInt>;
        using GuiSliderIntPtr = SliderInt *;

        class InputText : public Object
        {
        private:
            std::string m_name;
            size_t m_max_length = 0;
            char *m_text = nullptr;
            ImGuiInputTextFlags m_flags = 0;

        public:
            InputText(const std::string& name, size_t max_length, const std::string& default_value = "")
                : m_name(name), m_max_length(max_length), m_text(new char[max_length + 1])
            {
                set_value(default_value);
            }

            virtual void update() override
            {
                ImGui::InputText(m_name.c_str(), m_text, m_max_length);
            }

            InputText& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            InputText& set_value(const std::string& value)
            {
                size_t max_copy = std::min(value.size(), m_max_length);
                memcpy(m_text, value.c_str(), max_copy);
                m_text[max_copy] = '\0';
                return *this;
            }

            InputText& set_max_length(size_t max_length)
            {
                m_max_length = max_length;
                return *this;
            }

            InputText& set_flags(ImGuiInputTextFlags flags)
            {
                m_flags = flags;
                return *this;
            }

            ~InputText()
            {
                delete[] m_text;
            }

            const char *get_ctext() const
            {
                return m_text;
            }

            std::string get_text() const
            {
                return std::string(m_text, m_max_length);
            }
        };

        using GuiInputText = std::shared_ptr<InputText>;
        using GuiInputTextPtr = InputText *;

        class Checkbox : public Object
        {
        private:
            std::string m_name;
            bool m_value = false;

        public:
            Checkbox(const std::string& name, bool value = false)
                : m_name(name), m_value(value)
            {
            }

            virtual void update() override
            {
                ImGui::Checkbox(m_name.c_str(), &m_value);
            }

            Checkbox& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            Checkbox& set_value(bool value)
            {
                m_value = value;
                return *this;
            }

            bool get_value() const
            {
                return m_value;
            }
        };

        using GuiCheckbox = std::shared_ptr<Checkbox>;
        using GuiCheckboxPtr = Checkbox *;

        class Combo : public Object
        {
        private:
            std::string m_name;
            int m_current_item = 0;
            std::vector<char *> m_items;

        public:
            Combo(const std::string& name, int current_item = 0)
                : m_name(name), m_current_item(current_item)
            {
            }

            virtual void update() override
            {
                ImGui::Combo(m_name.c_str(), &m_current_item, m_items.data(), m_items.size());
            }

            Combo& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            void clear()
            {
                for (auto& item : m_items)
                {
                    delete[] item;
                }
                m_items.clear();
            }

            Combo& add_items(const std::vector<std::string>& items)
            {
                clear();
                for (auto& item : items)
                {
                    add_item(item);
                }
                m_current_item = 0;
                return *this;
            }

            Combo& add_item(const std::string& item)
            {
                char* cstr = new char[item.size() + 1];
                memcpy(cstr, item.c_str(), item.size());
                cstr[item.size()] = '\0';

                m_items.push_back(cstr);
                return *this;
            }

            Combo& set_current_item_index(int current_item)
            {
                m_current_item = current_item;
                return *this;
            }

            int get_current_item_index() const
            {
                return m_current_item;
            }

            const char *get_current_item() const
            {
                if ((unsigned int)m_current_item >= m_items.size()) {
                    return nullptr;
                }
                return m_items[m_current_item];
            }

            const std::vector<char *>& get_items() const
            {
                return m_items;
            }

            ~Combo()
            {
                clear();
            }
        };
        
        using GuiCombo = std::shared_ptr<Combo>;
        using GuiComboPtr = Combo *;
        
        class FuzzyCombo : public Object
        {
        private:
            std::string m_name;
            InputText m_input_text;
            std::vector<std::string> m_items;
            Combo m_filtered_combo;
        
        public:
            FuzzyCombo(const std::string& name, int current_item = 0)
                : m_name(name)
                , m_input_text(name, 256)
                , m_filtered_combo(name, current_item)
            {
            }

            InputText& get_input_text()
            {
                return m_input_text;
            }

            void set_items(const std::vector<std::string>& items)
            {
                m_items = items;
            }

            virtual void update() override
            {
                ImGui::BeginGroup();
                m_input_text.update();

                std::string input_text = m_input_text.get_text();
                std::transform(input_text.begin(), input_text.end(), input_text.begin(), ::tolower);

                static const float SCORE_EQUAL = 10;
                static const float SCORE_NOT_SAME = -15;

                std::vector<std::pair<std::string, float>> valids;

                for (auto& item : m_items) {
                    float score = 0;
                    float max_score = SCORE_EQUAL * std::min(input_text.size(), item.size());
                    std::string item_str = item;

                    std::transform(item_str.begin(), item_str.end(), item_str.begin(), ::tolower);
                    // fuzzy search
                    for (size_t i = 0; i < input_text.size(); i++) {
                        char c = input_text[i];
                        auto it = std::find(item_str.begin(), item_str.end(), c);
                        if (it != item_str.end()) {
                            score += SCORE_EQUAL;
                            item_str.erase(it);
                        } else {
                            score += SCORE_NOT_SAME;
                        }
                    }
                    float score_percent = score / max_score;
                    if (score_percent > 0.60f) {
                        auto it = std::find_if(valids.begin(), valids.end(), [&](const std::pair<std::string, float>& p) {
                            return p.second < score_percent;
                        });
                        if (it != valids.end()) {
                            valids.insert(it, std::make_pair(item, score_percent));
                        } else {
                            valids.push_back(std::make_pair(item, score_percent));
                        }
                    }
                }

                m_filtered_combo.clear();
                for (auto& item : valids) {
                    m_filtered_combo.add_item(item.first);
                }
                m_filtered_combo.set_current_item_index(0);
                m_filtered_combo.update();

                ImGui::EndGroup();
            }

            Combo& get_filtered_combo()
            {
                return m_filtered_combo;
            }
        };

        using GuiFuzzyCombo = std::shared_ptr<FuzzyCombo>;
        using GuiFuzzyComboPtr = FuzzyCombo *;

        class Child : public Object
        {
        private:
            std::string m_name;
            std::vector<GuiObject> m_children;
        
        public:
            Child(const std::string& name)
                : m_name(name)
            {
            }

            virtual void update() override
            {
                ImGui::BeginChild(m_name.c_str());
                for (auto& child : m_children)
                {
                    child->update();
                }
                ImGui::EndChild();
            }

            Child& set_name(const std::string& name)
            {
                m_name = name;
                return *this;
            }

            Child& add_child(const GuiObject& child)
            {
                m_children.push_back(child);
                return *this;
            }

            Child& add_child(GuiObjectPtr child)
            {
                m_children.push_back(GuiObject(child));
                return *this;
            }
        };

        using GuiChild = std::shared_ptr<Child>;
        using GuiChildPtr = Child *;
    }
}
