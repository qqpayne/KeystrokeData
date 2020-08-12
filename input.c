#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NAME_ELEMENT(element) [element] = #element

static const char * const keys[KEY_MAX + 1];

void INThandler(){
    exit(0);
}

int main(){
    char keyboard[] = "/dev/input/event3";
    char logFile[] = "/home/payne/workfolder/input/keyhistory.csv";
    
    FILE *log = fopen(logFile, "a");
    int device = open(keyboard, O_RDONLY);

    struct input_event ev;
    time_t gmt_ts;
    
    signal(SIGINT, INThandler);

    const char* newKeys[KEY_MAX + 1]; // массив нормальных названий клавиш (без KEY_)
    for (int i = 0; i < KEY_MAX; i++){
        if (keys[i] != NULL){
            int sz = strlen(keys[i]);
            char *text = (char*) malloc((sz-3)*sizeof(char));
            strcpy(text, keys[i]);
            memmove(text, text+4, sz-4); // двигаем на 4 символа вперед (обрезаем KEY_)
            text[sz-4] = '\0';
            newKeys[i] = text;
        }
    }
    
    while(1){

        read(device, &ev, sizeof(ev));

        if(ev.type == 1 && ev.value == 1){ // принимаем только keypress эвенты
            gmt_ts = mktime(gmtime(&ev.time.tv_sec)); 
            // по какой-то странной причине localtime возвращает время по GMT, 
            // а gmtime возвращает время GMT, из которого вычтено местное смещение (т.е если я в GMT+3, мне вернет GMT-3)
            // поэтому приходится прибегать к хаку для получения местного времени
            // разница между GMT (epoch time) и фейк-GMT (после gmtime) - это как раз то смещение, которое нужно 
            // добавить к GMT для получения местного времени
            fprintf(log, "%d,%s\n", (ev.time.tv_sec+(ev.time.tv_sec - gmt_ts)), newKeys[ev.code]);
            fflush(log); // сразу же записываем нажатие в лог
        }
    }
}

// переписываем /usr/include/linux/input-event-codes.h в обратную сторону (код взят из evtest)
static const char * const keys[KEY_MAX + 1] = {
    [0 ... KEY_MAX] = NULL,
    NAME_ELEMENT(KEY_RESERVED),		NAME_ELEMENT(KEY_ESC),
    NAME_ELEMENT(KEY_1),			NAME_ELEMENT(KEY_2),
    NAME_ELEMENT(KEY_3),			NAME_ELEMENT(KEY_4),
    NAME_ELEMENT(KEY_5),			NAME_ELEMENT(KEY_6),
    NAME_ELEMENT(KEY_7),			NAME_ELEMENT(KEY_8),
    NAME_ELEMENT(KEY_9),			NAME_ELEMENT(KEY_0),
    NAME_ELEMENT(KEY_MINUS),		NAME_ELEMENT(KEY_EQUAL),
    NAME_ELEMENT(KEY_BACKSPACE),		NAME_ELEMENT(KEY_TAB),
    NAME_ELEMENT(KEY_Q),			NAME_ELEMENT(KEY_W),
    NAME_ELEMENT(KEY_E),			NAME_ELEMENT(KEY_R),
    NAME_ELEMENT(KEY_T),			NAME_ELEMENT(KEY_Y),
    NAME_ELEMENT(KEY_U),			NAME_ELEMENT(KEY_I),
    NAME_ELEMENT(KEY_O),			NAME_ELEMENT(KEY_P),
    NAME_ELEMENT(KEY_LEFTBRACE),		NAME_ELEMENT(KEY_RIGHTBRACE),
    NAME_ELEMENT(KEY_ENTER),		NAME_ELEMENT(KEY_LEFTCTRL),
    NAME_ELEMENT(KEY_A),			NAME_ELEMENT(KEY_S),
    NAME_ELEMENT(KEY_D),			NAME_ELEMENT(KEY_F),
    NAME_ELEMENT(KEY_G),			NAME_ELEMENT(KEY_H),
    NAME_ELEMENT(KEY_J),			NAME_ELEMENT(KEY_K),
    NAME_ELEMENT(KEY_L),			NAME_ELEMENT(KEY_SEMICOLON),
    NAME_ELEMENT(KEY_APOSTROPHE),		NAME_ELEMENT(KEY_GRAVE),
    NAME_ELEMENT(KEY_LEFTSHIFT),		NAME_ELEMENT(KEY_BACKSLASH),
    NAME_ELEMENT(KEY_Z),			NAME_ELEMENT(KEY_X),
    NAME_ELEMENT(KEY_C),			NAME_ELEMENT(KEY_V),
    NAME_ELEMENT(KEY_B),			NAME_ELEMENT(KEY_N),
    NAME_ELEMENT(KEY_M),			NAME_ELEMENT(KEY_COMMA),
    NAME_ELEMENT(KEY_DOT),			NAME_ELEMENT(KEY_SLASH),
    NAME_ELEMENT(KEY_RIGHTSHIFT),		NAME_ELEMENT(KEY_KPASTERISK),
    NAME_ELEMENT(KEY_LEFTALT),		NAME_ELEMENT(KEY_SPACE),
    NAME_ELEMENT(KEY_CAPSLOCK),		NAME_ELEMENT(KEY_F1),
    NAME_ELEMENT(KEY_F2),			NAME_ELEMENT(KEY_F3),
    NAME_ELEMENT(KEY_F4),			NAME_ELEMENT(KEY_F5),
    NAME_ELEMENT(KEY_F6),			NAME_ELEMENT(KEY_F7),
    NAME_ELEMENT(KEY_F8),			NAME_ELEMENT(KEY_F9),
    NAME_ELEMENT(KEY_F10),			NAME_ELEMENT(KEY_NUMLOCK),
    NAME_ELEMENT(KEY_SCROLLLOCK),		NAME_ELEMENT(KEY_KP7),
    NAME_ELEMENT(KEY_KP8),			NAME_ELEMENT(KEY_KP9),
    NAME_ELEMENT(KEY_KPMINUS),		NAME_ELEMENT(KEY_KP4),
    NAME_ELEMENT(KEY_KP5),			NAME_ELEMENT(KEY_KP6),
    NAME_ELEMENT(KEY_KPPLUS),		NAME_ELEMENT(KEY_KP1),
    NAME_ELEMENT(KEY_KP2),			NAME_ELEMENT(KEY_KP3),
    NAME_ELEMENT(KEY_KP0),			NAME_ELEMENT(KEY_KPDOT),
    NAME_ELEMENT(KEY_ZENKAKUHANKAKU), 	NAME_ELEMENT(KEY_102ND),
    NAME_ELEMENT(KEY_F11),			NAME_ELEMENT(KEY_F12),
    NAME_ELEMENT(KEY_RO),			NAME_ELEMENT(KEY_KATAKANA),
    NAME_ELEMENT(KEY_HIRAGANA),		NAME_ELEMENT(KEY_HENKAN),
    NAME_ELEMENT(KEY_KATAKANAHIRAGANA),	NAME_ELEMENT(KEY_MUHENKAN),
    NAME_ELEMENT(KEY_KPJPCOMMA),		NAME_ELEMENT(KEY_KPENTER),
    NAME_ELEMENT(KEY_RIGHTCTRL),		NAME_ELEMENT(KEY_KPSLASH),
    NAME_ELEMENT(KEY_SYSRQ),		NAME_ELEMENT(KEY_RIGHTALT),
    NAME_ELEMENT(KEY_LINEFEED),		NAME_ELEMENT(KEY_HOME),
    NAME_ELEMENT(KEY_UP),			NAME_ELEMENT(KEY_PAGEUP),
    NAME_ELEMENT(KEY_LEFT),			NAME_ELEMENT(KEY_RIGHT),
    NAME_ELEMENT(KEY_END),			NAME_ELEMENT(KEY_DOWN),
    NAME_ELEMENT(KEY_PAGEDOWN),		NAME_ELEMENT(KEY_INSERT),
    NAME_ELEMENT(KEY_DELETE),		NAME_ELEMENT(KEY_MACRO),
    NAME_ELEMENT(KEY_MUTE),			NAME_ELEMENT(KEY_VOLUMEDOWN),
    NAME_ELEMENT(KEY_VOLUMEUP),		NAME_ELEMENT(KEY_POWER),
    NAME_ELEMENT(KEY_KPEQUAL),		NAME_ELEMENT(KEY_KPPLUSMINUS),
    NAME_ELEMENT(KEY_PAUSE),		NAME_ELEMENT(KEY_KPCOMMA),
    NAME_ELEMENT(KEY_HANGUEL),		NAME_ELEMENT(KEY_HANJA),
    NAME_ELEMENT(KEY_YEN),			NAME_ELEMENT(KEY_LEFTMETA),
    NAME_ELEMENT(KEY_RIGHTMETA),		NAME_ELEMENT(KEY_COMPOSE),
    NAME_ELEMENT(KEY_STOP),			NAME_ELEMENT(KEY_AGAIN),
    NAME_ELEMENT(KEY_PROPS),		NAME_ELEMENT(KEY_UNDO),
    NAME_ELEMENT(KEY_FRONT),		NAME_ELEMENT(KEY_COPY),
    NAME_ELEMENT(KEY_OPEN),			NAME_ELEMENT(KEY_PASTE),
    NAME_ELEMENT(KEY_FIND),			NAME_ELEMENT(KEY_CUT),
    NAME_ELEMENT(KEY_HELP),			NAME_ELEMENT(KEY_MENU),
    NAME_ELEMENT(KEY_CALC),			NAME_ELEMENT(KEY_SETUP),
    NAME_ELEMENT(KEY_SLEEP),		NAME_ELEMENT(KEY_WAKEUP),
    NAME_ELEMENT(KEY_FILE),			NAME_ELEMENT(KEY_SENDFILE),
    NAME_ELEMENT(KEY_DELETEFILE),		NAME_ELEMENT(KEY_XFER),
    NAME_ELEMENT(KEY_PROG1),		NAME_ELEMENT(KEY_PROG2),
    NAME_ELEMENT(KEY_WWW),			NAME_ELEMENT(KEY_MSDOS),
    NAME_ELEMENT(KEY_COFFEE),		NAME_ELEMENT(KEY_DIRECTION),
    NAME_ELEMENT(KEY_CYCLEWINDOWS),		NAME_ELEMENT(KEY_MAIL),
    NAME_ELEMENT(KEY_BOOKMARKS),		NAME_ELEMENT(KEY_COMPUTER),
    NAME_ELEMENT(KEY_BACK),			NAME_ELEMENT(KEY_FORWARD),
    NAME_ELEMENT(KEY_CLOSECD),		NAME_ELEMENT(KEY_EJECTCD),
    NAME_ELEMENT(KEY_EJECTCLOSECD),		NAME_ELEMENT(KEY_NEXTSONG),
    NAME_ELEMENT(KEY_PLAYPAUSE),		NAME_ELEMENT(KEY_PREVIOUSSONG),
    NAME_ELEMENT(KEY_STOPCD),		NAME_ELEMENT(KEY_RECORD),
    NAME_ELEMENT(KEY_REWIND),		NAME_ELEMENT(KEY_PHONE),
    NAME_ELEMENT(KEY_ISO),			NAME_ELEMENT(KEY_CONFIG),
    NAME_ELEMENT(KEY_HOMEPAGE),		NAME_ELEMENT(KEY_REFRESH),
    NAME_ELEMENT(KEY_EXIT),			NAME_ELEMENT(KEY_MOVE),
    NAME_ELEMENT(KEY_EDIT),			NAME_ELEMENT(KEY_SCROLLUP),
    NAME_ELEMENT(KEY_SCROLLDOWN),		NAME_ELEMENT(KEY_KPLEFTPAREN),
    NAME_ELEMENT(KEY_KPRIGHTPAREN), 	NAME_ELEMENT(KEY_F13),
    NAME_ELEMENT(KEY_F14),			NAME_ELEMENT(KEY_F15),
    NAME_ELEMENT(KEY_F16),			NAME_ELEMENT(KEY_F17),
    NAME_ELEMENT(KEY_F18),			NAME_ELEMENT(KEY_F19),
    NAME_ELEMENT(KEY_F20),			NAME_ELEMENT(KEY_F21),
    NAME_ELEMENT(KEY_F22),			NAME_ELEMENT(KEY_F23),
    NAME_ELEMENT(KEY_F24),			NAME_ELEMENT(KEY_PLAYCD),
    NAME_ELEMENT(KEY_PAUSECD),		NAME_ELEMENT(KEY_PROG3),
    NAME_ELEMENT(KEY_PROG4),		NAME_ELEMENT(KEY_SUSPEND),
    NAME_ELEMENT(KEY_CLOSE),		NAME_ELEMENT(KEY_PLAY),
    NAME_ELEMENT(KEY_FASTFORWARD),		NAME_ELEMENT(KEY_BASSBOOST),
    NAME_ELEMENT(KEY_PRINT),		NAME_ELEMENT(KEY_HP),
    NAME_ELEMENT(KEY_CAMERA),		NAME_ELEMENT(KEY_SOUND),
    NAME_ELEMENT(KEY_QUESTION),		NAME_ELEMENT(KEY_EMAIL),
    NAME_ELEMENT(KEY_CHAT),			NAME_ELEMENT(KEY_SEARCH),
    NAME_ELEMENT(KEY_CONNECT),		NAME_ELEMENT(KEY_FINANCE),
    NAME_ELEMENT(KEY_SPORT),		NAME_ELEMENT(KEY_SHOP),
    NAME_ELEMENT(KEY_ALTERASE),		NAME_ELEMENT(KEY_CANCEL),
    NAME_ELEMENT(KEY_BRIGHTNESSDOWN),	NAME_ELEMENT(KEY_BRIGHTNESSUP),
    NAME_ELEMENT(KEY_MEDIA),		NAME_ELEMENT(KEY_UNKNOWN),
    NAME_ELEMENT(KEY_OK),
    NAME_ELEMENT(KEY_SELECT),		NAME_ELEMENT(KEY_GOTO),
    NAME_ELEMENT(KEY_CLEAR),		NAME_ELEMENT(KEY_POWER2),
    NAME_ELEMENT(KEY_OPTION),		NAME_ELEMENT(KEY_INFO),
    NAME_ELEMENT(KEY_TIME),			NAME_ELEMENT(KEY_VENDOR),
    NAME_ELEMENT(KEY_ARCHIVE),		NAME_ELEMENT(KEY_PROGRAM),
    NAME_ELEMENT(KEY_CHANNEL),		NAME_ELEMENT(KEY_FAVORITES),
    NAME_ELEMENT(KEY_EPG),			NAME_ELEMENT(KEY_PVR),
    NAME_ELEMENT(KEY_MHP),			NAME_ELEMENT(KEY_LANGUAGE),
    NAME_ELEMENT(KEY_TITLE),		NAME_ELEMENT(KEY_SUBTITLE),
    NAME_ELEMENT(KEY_ANGLE),		NAME_ELEMENT(KEY_ZOOM),
    NAME_ELEMENT(KEY_MODE),			NAME_ELEMENT(KEY_KEYBOARD),
    NAME_ELEMENT(KEY_SCREEN),		NAME_ELEMENT(KEY_PC),
    NAME_ELEMENT(KEY_TV),			NAME_ELEMENT(KEY_TV2),
    NAME_ELEMENT(KEY_VCR),			NAME_ELEMENT(KEY_VCR2),
    NAME_ELEMENT(KEY_SAT),			NAME_ELEMENT(KEY_SAT2),
    NAME_ELEMENT(KEY_CD),			NAME_ELEMENT(KEY_TAPE),
    NAME_ELEMENT(KEY_RADIO),		NAME_ELEMENT(KEY_TUNER),
    NAME_ELEMENT(KEY_PLAYER),		NAME_ELEMENT(KEY_TEXT),
    NAME_ELEMENT(KEY_DVD),			NAME_ELEMENT(KEY_AUX),
    NAME_ELEMENT(KEY_MP3),			NAME_ELEMENT(KEY_AUDIO),
    NAME_ELEMENT(KEY_VIDEO),		NAME_ELEMENT(KEY_DIRECTORY),
    NAME_ELEMENT(KEY_LIST),			NAME_ELEMENT(KEY_MEMO),
    NAME_ELEMENT(KEY_CALENDAR),		NAME_ELEMENT(KEY_RED),
    NAME_ELEMENT(KEY_GREEN),		NAME_ELEMENT(KEY_YELLOW),
    NAME_ELEMENT(KEY_BLUE),			NAME_ELEMENT(KEY_CHANNELUP),
    NAME_ELEMENT(KEY_CHANNELDOWN),		NAME_ELEMENT(KEY_FIRST),
    NAME_ELEMENT(KEY_LAST),			NAME_ELEMENT(KEY_AB),
    NAME_ELEMENT(KEY_NEXT),			NAME_ELEMENT(KEY_RESTART),
    NAME_ELEMENT(KEY_SLOW),			NAME_ELEMENT(KEY_SHUFFLE),
    NAME_ELEMENT(KEY_BREAK),		NAME_ELEMENT(KEY_PREVIOUS),
    NAME_ELEMENT(KEY_DIGITS),		NAME_ELEMENT(KEY_TEEN),
    NAME_ELEMENT(KEY_TWEN),			NAME_ELEMENT(KEY_DEL_EOL),
    NAME_ELEMENT(KEY_DEL_EOS),		NAME_ELEMENT(KEY_INS_LINE),
    NAME_ELEMENT(KEY_DEL_LINE),
    NAME_ELEMENT(KEY_VIDEOPHONE),		NAME_ELEMENT(KEY_GAMES),
    NAME_ELEMENT(KEY_ZOOMIN),		NAME_ELEMENT(KEY_ZOOMOUT),
    NAME_ELEMENT(KEY_ZOOMRESET),		NAME_ELEMENT(KEY_WORDPROCESSOR),
    NAME_ELEMENT(KEY_EDITOR),		NAME_ELEMENT(KEY_SPREADSHEET),
    NAME_ELEMENT(KEY_GRAPHICSEDITOR), 	NAME_ELEMENT(KEY_PRESENTATION),
    NAME_ELEMENT(KEY_DATABASE),		NAME_ELEMENT(KEY_NEWS),
    NAME_ELEMENT(KEY_VOICEMAIL),		NAME_ELEMENT(KEY_ADDRESSBOOK),
    NAME_ELEMENT(KEY_MESSENGER),		NAME_ELEMENT(KEY_DISPLAYTOGGLE),
#ifdef KEY_SPELLCHECK
    NAME_ELEMENT(KEY_SPELLCHECK),
#endif
#ifdef KEY_LOGOFF
    NAME_ELEMENT(KEY_LOGOFF),
#endif
#ifdef KEY_DOLLAR
    NAME_ELEMENT(KEY_DOLLAR),
#endif
#ifdef KEY_EURO
    NAME_ELEMENT(KEY_EURO),
#endif
#ifdef KEY_FRAMEBACK
    NAME_ELEMENT(KEY_FRAMEBACK),
#endif
#ifdef KEY_FRAMEFORWARD
    NAME_ELEMENT(KEY_FRAMEFORWARD),
#endif
#ifdef KEY_CONTEXT_MENU
    NAME_ELEMENT(KEY_CONTEXT_MENU),
#endif
#ifdef KEY_MEDIA_REPEAT
    NAME_ELEMENT(KEY_MEDIA_REPEAT),
#endif
#ifdef KEY_10CHANNELSUP
    NAME_ELEMENT(KEY_10CHANNELSUP),
#endif
#ifdef KEY_10CHANNELSDOWN
    NAME_ELEMENT(KEY_10CHANNELSDOWN),
#endif
#ifdef KEY_IMAGES
    NAME_ELEMENT(KEY_IMAGES),
#endif
    NAME_ELEMENT(KEY_DEL_EOL),		NAME_ELEMENT(KEY_DEL_EOS),
    NAME_ELEMENT(KEY_INS_LINE),	 	NAME_ELEMENT(KEY_DEL_LINE),
    NAME_ELEMENT(KEY_FN),			NAME_ELEMENT(KEY_FN_ESC),
    NAME_ELEMENT(KEY_FN_F1),		NAME_ELEMENT(KEY_FN_F2),
    NAME_ELEMENT(KEY_FN_F3),		NAME_ELEMENT(KEY_FN_F4),
    NAME_ELEMENT(KEY_FN_F5),		NAME_ELEMENT(KEY_FN_F6),
    NAME_ELEMENT(KEY_FN_F7),		NAME_ELEMENT(KEY_FN_F8),
    NAME_ELEMENT(KEY_FN_F9),		NAME_ELEMENT(KEY_FN_F10),
    NAME_ELEMENT(KEY_FN_F11),		NAME_ELEMENT(KEY_FN_F12),
    NAME_ELEMENT(KEY_FN_1),			NAME_ELEMENT(KEY_FN_2),
    NAME_ELEMENT(KEY_FN_D),			NAME_ELEMENT(KEY_FN_E),
    NAME_ELEMENT(KEY_FN_F),			NAME_ELEMENT(KEY_FN_S),
    NAME_ELEMENT(KEY_FN_B),
    NAME_ELEMENT(KEY_BRL_DOT1),		NAME_ELEMENT(KEY_BRL_DOT2),
    NAME_ELEMENT(KEY_BRL_DOT3),		NAME_ELEMENT(KEY_BRL_DOT4),
    NAME_ELEMENT(KEY_BRL_DOT5),		NAME_ELEMENT(KEY_BRL_DOT6),
    NAME_ELEMENT(KEY_BRL_DOT7),		NAME_ELEMENT(KEY_BRL_DOT8),
    NAME_ELEMENT(KEY_BRL_DOT9),		NAME_ELEMENT(KEY_BRL_DOT10),
#ifdef KEY_NUMERIC_0
    NAME_ELEMENT(KEY_NUMERIC_0),		NAME_ELEMENT(KEY_NUMERIC_1),
    NAME_ELEMENT(KEY_NUMERIC_2),		NAME_ELEMENT(KEY_NUMERIC_3),
    NAME_ELEMENT(KEY_NUMERIC_4),		NAME_ELEMENT(KEY_NUMERIC_5),
    NAME_ELEMENT(KEY_NUMERIC_6),		NAME_ELEMENT(KEY_NUMERIC_7),
    NAME_ELEMENT(KEY_NUMERIC_8),		NAME_ELEMENT(KEY_NUMERIC_9),
    NAME_ELEMENT(KEY_NUMERIC_STAR),		NAME_ELEMENT(KEY_NUMERIC_POUND),
#endif
    NAME_ELEMENT(KEY_BATTERY),
    NAME_ELEMENT(KEY_BLUETOOTH),		NAME_ELEMENT(KEY_BRIGHTNESS_CYCLE),
    NAME_ELEMENT(KEY_BRIGHTNESS_ZERO),
#ifdef KEY_DASHBOARD
    NAME_ELEMENT(KEY_DASHBOARD),
#endif
    NAME_ELEMENT(KEY_DISPLAY_OFF),		NAME_ELEMENT(KEY_DOCUMENTS),
    NAME_ELEMENT(KEY_FORWARDMAIL),		NAME_ELEMENT(KEY_NEW),
    NAME_ELEMENT(KEY_KBDILLUMDOWN),		NAME_ELEMENT(KEY_KBDILLUMUP),
    NAME_ELEMENT(KEY_KBDILLUMTOGGLE), 	NAME_ELEMENT(KEY_REDO),
    NAME_ELEMENT(KEY_REPLY),		NAME_ELEMENT(KEY_SAVE),
#ifdef KEY_SCALE
    NAME_ELEMENT(KEY_SCALE),
#endif
    NAME_ELEMENT(KEY_SEND),
    NAME_ELEMENT(KEY_SCREENLOCK),		NAME_ELEMENT(KEY_SWITCHVIDEOMODE),
#ifdef KEY_UWB
    NAME_ELEMENT(KEY_UWB),
#endif
#ifdef KEY_VIDEO_NEXT
    NAME_ELEMENT(KEY_VIDEO_NEXT),
#endif
#ifdef KEY_VIDEO_PREV
    NAME_ELEMENT(KEY_VIDEO_PREV),
#endif
#ifdef KEY_WIMAX
    NAME_ELEMENT(KEY_WIMAX),
#endif
#ifdef KEY_WLAN
    NAME_ELEMENT(KEY_WLAN),
#endif
#ifdef KEY_RFKILL
    NAME_ELEMENT(KEY_RFKILL),
#endif
#ifdef KEY_MICMUTE
    NAME_ELEMENT(KEY_MICMUTE),
#endif
#ifdef KEY_CAMERA_FOCUS
    NAME_ELEMENT(KEY_CAMERA_FOCUS),
#endif
#ifdef KEY_WPS_BUTTON
    NAME_ELEMENT(KEY_WPS_BUTTON),
#endif
#ifdef KEY_TOUCHPAD_TOGGLE
    NAME_ELEMENT(KEY_TOUCHPAD_TOGGLE),
    NAME_ELEMENT(KEY_TOUCHPAD_ON),
    NAME_ELEMENT(KEY_TOUCHPAD_OFF),
#endif
#ifdef KEY_CAMERA_ZOOMIN
    NAME_ELEMENT(KEY_CAMERA_ZOOMIN),	NAME_ELEMENT(KEY_CAMERA_ZOOMOUT),
    NAME_ELEMENT(KEY_CAMERA_UP),		NAME_ELEMENT(KEY_CAMERA_DOWN),
    NAME_ELEMENT(KEY_CAMERA_LEFT),		NAME_ELEMENT(KEY_CAMERA_RIGHT),
#endif
#ifdef KEY_ATTENDANT_ON
    NAME_ELEMENT(KEY_ATTENDANT_ON),		NAME_ELEMENT(KEY_ATTENDANT_OFF),
    NAME_ELEMENT(KEY_ATTENDANT_TOGGLE),	NAME_ELEMENT(KEY_LIGHTS_TOGGLE),
#endif

    NAME_ELEMENT(BTN_0),			NAME_ELEMENT(BTN_1),
    NAME_ELEMENT(BTN_2),			NAME_ELEMENT(BTN_3),
    NAME_ELEMENT(BTN_4),			NAME_ELEMENT(BTN_5),
    NAME_ELEMENT(BTN_6),			NAME_ELEMENT(BTN_7),
    NAME_ELEMENT(BTN_8),			NAME_ELEMENT(BTN_9),
    NAME_ELEMENT(BTN_LEFT),			NAME_ELEMENT(BTN_RIGHT),
    NAME_ELEMENT(BTN_MIDDLE),		NAME_ELEMENT(BTN_SIDE),
    NAME_ELEMENT(BTN_EXTRA),		NAME_ELEMENT(BTN_FORWARD),
    NAME_ELEMENT(BTN_BACK),			NAME_ELEMENT(BTN_TASK),
    NAME_ELEMENT(BTN_TRIGGER),		NAME_ELEMENT(BTN_THUMB),
    NAME_ELEMENT(BTN_THUMB2),		NAME_ELEMENT(BTN_TOP),
    NAME_ELEMENT(BTN_TOP2),			NAME_ELEMENT(BTN_PINKIE),
    NAME_ELEMENT(BTN_BASE),			NAME_ELEMENT(BTN_BASE2),
    NAME_ELEMENT(BTN_BASE3),		NAME_ELEMENT(BTN_BASE4),
    NAME_ELEMENT(BTN_BASE5),		NAME_ELEMENT(BTN_BASE6),
    NAME_ELEMENT(BTN_DEAD),			NAME_ELEMENT(BTN_C),
#ifdef BTN_SOUTH
    NAME_ELEMENT(BTN_SOUTH),		NAME_ELEMENT(BTN_EAST),
    NAME_ELEMENT(BTN_NORTH),		NAME_ELEMENT(BTN_WEST),
#else
    NAME_ELEMENT(BTN_A),			NAME_ELEMENT(BTN_B),
    NAME_ELEMENT(BTN_X),			NAME_ELEMENT(BTN_Y),
#endif
    NAME_ELEMENT(BTN_Z),			NAME_ELEMENT(BTN_TL),
    NAME_ELEMENT(BTN_TR),			NAME_ELEMENT(BTN_TL2),
    NAME_ELEMENT(BTN_TR2),			NAME_ELEMENT(BTN_SELECT),
    NAME_ELEMENT(BTN_START),		NAME_ELEMENT(BTN_MODE),
    NAME_ELEMENT(BTN_THUMBL),		NAME_ELEMENT(BTN_THUMBR),
    NAME_ELEMENT(BTN_TOOL_PEN),		NAME_ELEMENT(BTN_TOOL_RUBBER),
    NAME_ELEMENT(BTN_TOOL_BRUSH),		NAME_ELEMENT(BTN_TOOL_PENCIL),
    NAME_ELEMENT(BTN_TOOL_AIRBRUSH),	NAME_ELEMENT(BTN_TOOL_FINGER),
    NAME_ELEMENT(BTN_TOOL_MOUSE),		NAME_ELEMENT(BTN_TOOL_LENS),
    NAME_ELEMENT(BTN_TOUCH),		NAME_ELEMENT(BTN_STYLUS),
    NAME_ELEMENT(BTN_STYLUS2),		NAME_ELEMENT(BTN_TOOL_DOUBLETAP),
    NAME_ELEMENT(BTN_TOOL_TRIPLETAP),
#ifdef BTN_TOOL_QUADTAP
    NAME_ELEMENT(BTN_TOOL_QUADTAP),
#endif
    NAME_ELEMENT(BTN_GEAR_DOWN),
    NAME_ELEMENT(BTN_GEAR_UP),

#ifdef BTN_DPAD_UP
    NAME_ELEMENT(BTN_DPAD_UP),		NAME_ELEMENT(BTN_DPAD_DOWN),
    NAME_ELEMENT(BTN_DPAD_LEFT),		NAME_ELEMENT(BTN_DPAD_RIGHT),
#endif
#ifdef KEY_ALS_TOGGLE
    NAME_ELEMENT(KEY_ALS_TOGGLE),
#endif
#ifdef KEY_BUTTONCONFIG
    NAME_ELEMENT(KEY_BUTTONCONFIG),
#endif
#ifdef KEY_TASKMANAGER
    NAME_ELEMENT(KEY_TASKMANAGER),
#endif
#ifdef KEY_JOURNAL
    NAME_ELEMENT(KEY_JOURNAL),
#endif
#ifdef KEY_CONTROLPANEL
    NAME_ELEMENT(KEY_CONTROLPANEL),
#endif
#ifdef KEY_APPSELECT
    NAME_ELEMENT(KEY_APPSELECT),
#endif
#ifdef KEY_SCREENSAVER
    NAME_ELEMENT(KEY_SCREENSAVER),
#endif
#ifdef KEY_VOICECOMMAND
    NAME_ELEMENT(KEY_VOICECOMMAND),
#endif
#ifdef KEY_BRIGHTNESS_MIN
    NAME_ELEMENT(KEY_BRIGHTNESS_MIN),
#endif
#ifdef KEY_BRIGHTNESS_MAX
    NAME_ELEMENT(KEY_BRIGHTNESS_MAX),
#endif
#ifdef KEY_KBDINPUTASSIST_PREV
    NAME_ELEMENT(KEY_KBDINPUTASSIST_PREV),
#endif
#ifdef KEY_KBDINPUTASSIST_NEXT
    NAME_ELEMENT(KEY_KBDINPUTASSIST_NEXT),
#endif
#ifdef KEY_KBDINPUTASSIST_PREVGROUP
    NAME_ELEMENT(KEY_KBDINPUTASSIST_PREVGROUP),
#endif
#ifdef KEY_KBDINPUTASSIST_NEXTGROUP
    NAME_ELEMENT(KEY_KBDINPUTASSIST_NEXTGROUP),
#endif
#ifdef KEY_KBDINPUTASSIST_ACCEPT
    NAME_ELEMENT(KEY_KBDINPUTASSIST_ACCEPT),
#endif
#ifdef KEY_KBDINPUTASSIST_CANCEL
    NAME_ELEMENT(KEY_KBDINPUTASSIST_CANCEL),
#endif
#ifdef BTN_TRIGGER_HAPPY
    NAME_ELEMENT(BTN_TRIGGER_HAPPY1),	NAME_ELEMENT(BTN_TRIGGER_HAPPY11),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY2),	NAME_ELEMENT(BTN_TRIGGER_HAPPY12),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY3),	NAME_ELEMENT(BTN_TRIGGER_HAPPY13),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY4),	NAME_ELEMENT(BTN_TRIGGER_HAPPY14),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY5),	NAME_ELEMENT(BTN_TRIGGER_HAPPY15),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY6),	NAME_ELEMENT(BTN_TRIGGER_HAPPY16),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY7),	NAME_ELEMENT(BTN_TRIGGER_HAPPY17),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY8),	NAME_ELEMENT(BTN_TRIGGER_HAPPY18),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY9),	NAME_ELEMENT(BTN_TRIGGER_HAPPY19),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY10),	NAME_ELEMENT(BTN_TRIGGER_HAPPY20),

    NAME_ELEMENT(BTN_TRIGGER_HAPPY21),	NAME_ELEMENT(BTN_TRIGGER_HAPPY31),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY22),	NAME_ELEMENT(BTN_TRIGGER_HAPPY32),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY23),	NAME_ELEMENT(BTN_TRIGGER_HAPPY33),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY24),	NAME_ELEMENT(BTN_TRIGGER_HAPPY34),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY25),	NAME_ELEMENT(BTN_TRIGGER_HAPPY35),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY26),	NAME_ELEMENT(BTN_TRIGGER_HAPPY36),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY27),	NAME_ELEMENT(BTN_TRIGGER_HAPPY37),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY28),	NAME_ELEMENT(BTN_TRIGGER_HAPPY38),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY29),	NAME_ELEMENT(BTN_TRIGGER_HAPPY39),
    NAME_ELEMENT(BTN_TRIGGER_HAPPY30),	NAME_ELEMENT(BTN_TRIGGER_HAPPY40),
#endif
#ifdef BTN_TOOL_QUINTTAP
    NAME_ELEMENT(BTN_TOOL_QUINTTAP),
#endif
};
