#pragma once

#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QFile>

enum class FloatingBarPosition {
    TopLeft,
    TopCenter,
    TopRight
};


class ConfigManager : public QObject // 终于把这玩意配置文件模块分离出来的我：我滴……任务……完！成！辣！啊哈哈哈哈哈哈哈！啊哈哈哈哈哈哈哈！
{
    Q_OBJECT

private:

    static inline const QStringList language_code_list_default = {
        "aa", "ab", "ae", "af", "ak", "am", "an", "ar", "as", "av", "ay", "az",
        "ba", "be", "bg", "bh", "bi", "bm", "bn", "bo", "br", "bs", "ca", "ce",
        "ch", "co", "cr", "cs", "cu", "cv", "cy", "da", "de", "dv", "dz", "ee",
        "el", "en", "eo", "es", "et", "eu", "fa", "ff", "fi", "fj", "fo", "fr",
        "fy", "ga", "gd", "gl", "gn", "gu", "gv", "ha", "he", "hi", "ho", "hr",
        "ht", "hu", "hy", "hz", "ia", "id", "ie", "ig", "ii", "ik", "io", "is",
        "it", "iu", "ja", "jv", "ka", "kg", "ki", "kj", "kk", "kl", "km", "kn",
        "ko", "kr", "ks", "ku", "kv", "kw", "ky", "la", "lb", "lg", "li", "ln",
        "lo", "lt", "lu", "lv", "mg", "mh", "mi", "mk", "ml", "mn", "mo", "mr",
        "ms", "mt", "my", "na", "nb", "nd", "ne", "ng", "nl", "nn", "no", "nr",
        "nv", "ny", "oc", "oj", "om", "or", "os", "pa", "pi", "pl", "ps", "pt",
        "qu", "rm", "rn", "ro", "ru", "rw", "sa", "sc", "sd", "se", "sg", "sh",
        "si", "sk", "sl", "sm", "sn", "so", "sq", "sr", "ss", "st", "su", "sv",
        "sw", "ta", "te", "tg", "th", "ti", "tk", "tl", "tn", "to", "tr", "ts",
        "tt", "tw", "ty", "ug", "uk", "ur", "uz", "ve", "vi", "vo", "wa", "wo",
        "xh", "yi", "yo", "za", "zh-CN", "zh-HK", "zh-TW", "zh-Classic-Hans", "zh-Classic-Hant", "zu"
    }; // 语言代码列表（大部分为ISO 639-1）
    static inline const QStringList language_name_list_default = {
        // aa
        "Afaraf",
        // ab
        "аҧсуа бызшәа",
        // ae
        "avesta",
        // af
        "Afrikaans",
        // ak
        "Akan",
        // am
        "አማርኛ",
        // an
        "aragonés",
        // ar
        "العربية",
        // as
        "অসমীয়া",
        // av
        "авар мацӀ",
        // ay
        "aymar aru",
        // az
        "azərbaycan dili",
        // ba
        "башҡорт теле",
        // be
        "беларуская мова",
        // bg
        "български език",
        // bh
        "भोजपुरी",
        // bi
        "Bislama",
        // bm
        "bamanankan",
        // bn
        "বাংলা",
        // bo
        "བོད་ཡིག",
        // br
        "brezhoneg",
        // bs
        "bosanski jezik",
        // ca
        "Català",
        // ce
        "нохчийн мотт",
        // ch
        "Chamoru",
        // co
        "corsu",
        // cr
        "ᓀᐦᐃᔭᐍᐏᐣ",
        // cs
        "čeština",
        // cu
        "ѩзыкъ словѣньскъ",
        // cv
        "чӑваш чӗлхи",
        // cy
        "Cymraeg",
        // da
        "dansk",
        // de
        "Deutsch",
        // dv
        "Dhivehi",
        // dz
        "རྫོང་ཁ",
        // ee
        "Eʋegbe",
        // el
        "Ελληνικά",
        // en
        "English",
        // eo
        "Esperanto",
        // es
        "Español",
        // et
        "eesti",
        // eu
        "euskara",
        // fa
        "فارسی",
        // ff
        "Fulfulde",
        // fi
        "suomi",
        // fj
        "Vakaviti",
        // fo
        "føroyskt",
        // fr
        "Français",
        // fy
        "Frysk",
        // ga
        "Gaeilge",
        // gd
        "Gàidhlig",
        // gl
        "galego",
        // gn
        "avañe'ẽ",
        // gu
        "ગુજરાતી",
        // gv
        "Gaelg",
        // ha
        "هَوُسَ",
        // he
        "עברית",
        // hi
        "हिन्दी",
        // ho
        "Hiri Motu",
        // hr
        "Hrvatski",
        // ht
        "Kreyòl ayisyen",
        // hu
        "magyar",
        // hy
        "Հայերեն",
        // hz
        "Otjiherero",
        // ia
        "Interlingua",
        // id
        "Bahasa Indonesia",
        // ie
        "Interlingue",
        // ig
        "Asụsụ Igbo",
        // ii
        "ꆈꌠ꒿ Nuosuhxop",
        // ik
        "Iñupiaq",
        // io
        "Ido",
        // is
        "Íslenska",
        // it
        "Italiano",
        // iu
        "ᐃᓄᒃᑎᑐᑦ",
        // ja
        "日本語",
        // jv
        "basa Jawa",
        // ka
        "ქართული",
        // kg
        "Kikongo",
        // ki
        "Gĩkũyũ",
        // kj
        "Kuanyama",
        // kk
        "қазақ тілі",
        // kl
        "kalaallisut",
        // km
        "ខេមរភាសា",
        // kn
        "ಕನ್ನಡ",
        // ko
        "한국어",
        // kr
        "Kanuri",
        // ks
        "कश्मीरी",
        // ku
        "Kurdî",
        // kv
        "коми кыв",
        // kw
        "Kernewek",
        // ky
        "Кыргызча",
        // la
        "latine",
        // lb
        "Lëtzebuergesch",
        // lg
        "Luganda",
        // li
        "Limburgs",
        // ln
        "Lingála",
        // lo
        "ພາສາ",
        // lt
        "lietuvių kalba",
        // lu
        "Tshiluba",
        // lv
        "latviešu valoda",
        // mg
        "fiteny malagasy",
        // mh
        "Kajin M̧ajeļ",
        // mi
        "te reo Māori",
        // mk
        "македонски јазик",
        // ml
        "മലയാളം",
        // mn
        "Монгол хэл",
        // mo
        "Лимба молдовеняскэ",
        // mr
        "मराठी",
        // ms
        "Bahasa Malaysia",
        // mt
        "Malti",
        // my
        "ဗမာစာ",
        // na
        "Ekakairũ Naoero",
        // nb
        "Norsk bokmål",
        // nd
        "isiNdebele",
        // ne
        "नेपाली",
        // ng
        "Owambo",
        // nl
        "Nederlands",
        // nn
        "Norsk nynorsk",
        // no
        "Norsk",
        // nr
        "isiNdebele",
        // nv
        "Diné bizaad",
        // ny
        "chiCheŵa",
        // oc
        "occitan",
        // oj
        "ᐊᓂᔑᓈᐯᒧᐎᓐ",
        // om
        "Afaan Oromoo",
        // or
        "ଓଡ଼ିଆ",
        // os
        "ирон æвзаг",
        // pa
        "ਪੰਜਾਬੀ",
        // pi
        "पाऴि",
        // pl
        "Polski",
        // ps
        "پښتو",
        // pt
        "Português",
        // qu
        "Runa Simi",
        // rm
        "rumantsch grischun",
        // rn
        "Ikirundi",
        // ro
        "Română",
        // ru
        "Русский",
        // rw
        "Ikinyarwanda",
        // sa
        "संस्कृतम्",
        // sc
        "sardu",
        // sd
        "सिन्धी",
        // se
        "Davvisámegiella",
        // sg
        "yângâ tî sängö",
        // sh
        "Српскохрватски језик",
        // si
        "සිංහල",
        // sk
        "slovenčina",
        // sl
        "slovenščina",
        // sm
        "Gagana Samoa",
        // sn
        "chiShona",
        // so
        "Soomaaliga",
        // sq
        "Shqip",
        // sr
        "српски језик",
        // ss
        "SiSwati",
        // st
        "Sesotho",
        // su
        "Basa Sunda",
        // sv
        "Svenska",
        // sw
        "Kiswahili",
        // ta
        "தமிழ்",
        // te
        "తెలుగు",
        // tg
        "тоҷикӣ",
        // th
        "ไทย",
        // ti
        "ትግርኛ",
        // tk
        "Türkmen",
        // tl
        "Wikang Tagalog",
        // tn
        "Setswana",
        // to
        "faka Tonga",
        // tr
        "Türkçe",
        // ts
        "Xitsonga",
        // tt
        "татар теле",
        // tw
        "Twi",
        // ty
        "Reo Tahiti",
        // ug
        "ئۇيغۇرچە‎",
        // uk
        "Українська",
        // ur
        "اردو",
        // uz
        "Ўзбек",
        // ve
        "Tshivenḓa",
        // vi
        "Tiếng Việt",
        // vo
        "Volapük",
        // wa
        "walon",
        // wo
        "Wollof",
        // xh
        "isiXhosa",
        // yi
        "ייִדיש",
        // yo
        "Yorùbá",
        // za
        "Saɯ cueŋƅ",
        // zh-CN
        "简体中文（中国大陆）",
        // zh-HK
        "繁體中文（中國香港）",
        // zh-TW
        "繁體中文（中國臺灣）",
        // zh-Classic-Hans
        "文言（简体）",
        // zh-Classic-Hant
        "文言（華夏）",
        // zu
        "isiZulu"
    };

    struct GeneralConfigItems {
        QDateTime target_date_time = QDateTime(QDate(2025, 6, 30), QTime(0, 0, 0)); // 目标时间，默认2025-06-30 00:00:00
        unsigned update_interval = 1000; // 更新间隔时间，默认1秒
        QString language = "zh-CN"; // 语言，默认中文
        QStringList language_code_list = language_code_list_default;
        QStringList language_name_list = language_name_list_default;
    };

    struct FloatingBarConfigItems {
        bool is_show_floating_bar = true; // 是否显示悬浮条，默认显示
        bool floating_bar_on_top = true; // 悬浮条是否置顶，默认置顶
        FloatingBarPosition floating_bar_position = FloatingBarPosition::TopCenter; // 悬浮条位置，默认居中
        unsigned floating_bar_border_radius = 10; // 悬浮条圆角，默认10
        unsigned floating_bar_height = 50; // 悬浮条高度，默认50
        QString floating_bar_text = "距会考还剩："; // 悬浮条文本
    };

    struct ReminderConfigItems {
        bool is_show_reminder = true; // 是否显示全屏提醒
        QString reminder_text = "距会考"; // 全屏提醒文本
        QString reminder_small_text = "THE EXAM IN "; // 全屏提醒小文本
        int remaining_days_to_play_countdown_sound = 30; // 剩余天数播放倒计时声音，默认30
        int remaining_days_to_play_heartbeat_sound = 14; // 剩余天数播放心跳声音，默认14
        unsigned block_show_times = 4; // 红色块显示次数，默认4
        QVariantList reminder_time_list = { QTime(8, 13), QTime(9, 3), QTime(9, 53), QTime(10, 43), QTime(11, 38), QTime(14, 20), QTime(15, 3), QTime(15, 53), QTime(16, 43), QTime(18, 3) }; // 时间列表
    };



public:
    ConfigManager(QObject* parent = nullptr);
    ~ConfigManager();

    GeneralConfigItems General;

    FloatingBarConfigItems FloatingBar;

    ReminderConfigItems Reminder;

    template<typename T1, typename T2> void set(T1& Item, const T2& Variant) {
        Item = Variant;
        write();
    }

    void read();
    void write();

};