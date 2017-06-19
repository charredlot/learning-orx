#include <assert.h>
#include <stdbool.h>

#include "orx.h"

#include "util.h"

enum keybind {
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
K_MOVE_MAX, /* all movement commands should be less than this */
    K_ZOOM_IN = K_MOVE_MAX,
    K_ZOOM_OUT,
    K_QUIT,
K_KEY_MAX,
};

static orxOBJECT *pig;
static float pigMoveSpeed = 60.0;

const char *keybinds[] = {
    [K_UP] = "Up",
    [K_DOWN] = "Down",
    [K_LEFT] = "Left",
    [K_RIGHT] = "Right",
    [K_ZOOM_IN] = "ZoomIn",
    [K_ZOOM_OUT] = "ZoomOut",
    [K_QUIT] = "Quit",
};

#define MOVE_PREFIX "Move"
const char *anim[] = {
    [K_UP] = MOVE_PREFIX "Up",
    [K_DOWN] = MOVE_PREFIX "Down",
    [K_LEFT] = MOVE_PREFIX "Left",
    [K_RIGHT] = MOVE_PREFIX "Right",
};

static orxSTATUS orxFASTCALL
animation_event_handler(const orxEVENT *event)
{
    orxANIM_EVENT_PAYLOAD *payload;

    /* Gets event payload */
    payload = (orxANIM_EVENT_PAYLOAD *)event->pstPayload;

    /* Depending on event type */
    switch(event->eID) {
        case orxANIM_EVENT_START:
            /* Logs info */
            orxLOG("Animation <%s>@<%s> has started!", payload->zAnimName,
                   orxObject_GetName(orxOBJECT(event->hRecipient)));

            break;
        case orxANIM_EVENT_STOP:
            orxLOG("Animation <%s>@<%s> has stopped!", payload->zAnimName,
                   orxObject_GetName(orxOBJECT(event->hRecipient)));

            break;
        case orxANIM_EVENT_CUT:
            orxLOG("Animation <%s>@<%s> has been cut!", payload->zAnimName,
                   orxObject_GetName(orxOBJECT(event->hRecipient)));
            break;
        case orxANIM_EVENT_LOOP:
            orxLOG("Animation <%s>@<%s> has looped!", payload->zAnimName,
                   orxObject_GetName(orxOBJECT(event->hRecipient)));

            break;
        case orxANIM_EVENT_CUSTOM_EVENT: {
            const char *name;
            float val;

            name = payload->stCustom.zName;
            val = payload->stCustom.fValue;

            orxLOG("Animation <%s>@<%s> has sent the event [%s] [%f]!", payload->zAnimName,
                   orxObject_GetName(orxOBJECT(event->hRecipient)), name, val);

            orxVECTOR pos;
            enum keybind k;
            bool found;

            /* hacky as fuck lol */
            found = false;
            for (k = 0; k < K_MOVE_MAX; k++) {
                if (strncmp(name, anim[k], strlen(anim[k])) != 0) {
                    continue;
                }

                orxObject_GetPosition(pig, &pos); 
                switch (k) {
                    case K_UP:
                        pos.fY -= pigMoveSpeed;
                        break;
                    case K_DOWN:
                        pos.fY += pigMoveSpeed;
                        break;
                    case K_LEFT:
                        pos.fX -= pigMoveSpeed;
                        break;
                    case K_RIGHT:
                        pos.fX += pigMoveSpeed;
                        break;
                    default:
                        assert(false);
                        break;
                }
                orxObject_SetPosition(pig, &pos);
                found = true;
                break;
            }

            if (!found) {
                orxLOG("unexpected custom event");
                assert(false);
            }
            break;
        }
    }

    return orxSTATUS_SUCCESS;
}

static void orxFASTCALL
input_timer(const orxCLOCK_INFO *_pstClockInfo, void *_pstContext)
{
    orxVECTOR vScale;
    enum keybind k;
    bool found;

    found = false;
    for (k = 0; k < K_MOVE_MAX; k++) {
        if (orxInput_IsActive(keybinds[k])) {
            orxObject_SetTargetAnim(pig, anim[k]);
            found = true;
            break;
        }
    }

    if (!found) {
        orxObject_SetTargetAnim(pig, orxNULL);
    }

    if (orxInput_IsActive(keybinds[K_ZOOM_IN])) {
        orxObject_SetScale(pig,
                           orxVector_Mulf(&vScale,
                                          orxObject_GetScale(pig, &vScale),
                                          orx2F(1.02f)));
    } else if(orxInput_IsActive(keybinds[K_ZOOM_OUT])) {
        orxObject_SetScale(pig,
                           orxVector_Mulf(&vScale,
                                          orxObject_GetScale(pig, &vScale),
                                          orx2F(0.98f)));
    }
}

static orxSTATUS orxFASTCALL
init(void)
{
    orxCLOCK *clock;
    int i;
    const orxSTRING buttons[K_KEY_MAX];

    for (i = 0; i < ARRAY_SIZE(keybinds); i++) {
        orxINPUT_TYPE   eType;
        orxENUM         eID;
        orxINPUT_MODE   eMode;

        orxInput_GetBinding(keybinds[i], 0, &eType, &eID, &eMode);
        buttons[i] = orxInput_GetBindingName(eType, eID, eMode);

        orxLOG("Key %s is mapped to %s", keybinds[i], buttons[i]);
    }

    orxEvent_AddHandler(orxEVENT_TYPE_ANIM, animation_event_handler);

    orxViewport_CreateFromConfig("Viewport");

    pig = orxObject_CreateFromConfig("Pig");

    /* TODO: what's the -1 for? */
    clock = orxClock_FindFirst(orx2F(-1.0f), orxCLOCK_TYPE_CORE);
    orxClock_Register(clock, input_timer, orxNULL, orxMODULE_ID_MAIN,
                      orxCLOCK_PRIORITY_NORMAL);

    /* Done! */
    return orxSTATUS_SUCCESS;
}

static orxSTATUS orxFASTCALL
run(void)
{
    orxSTATUS eResult = orxSTATUS_SUCCESS;

    if (orxInput_IsActive(keybinds[K_QUIT])) {
        eResult = orxSTATUS_FAILURE;
    }

    return eResult;
}

static void orxFASTCALL
uninit(void)
{
}

int
main(int argc, char **argv)
{
    orx_Execute(argc, argv, init, run, uninit);
    return EXIT_SUCCESS;
}


#ifdef __orxMSVC__

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  orx_WinExecute(Init, Run, Exit);
  return EXIT_SUCCESS;
}

#endif // __orxMSVC__
