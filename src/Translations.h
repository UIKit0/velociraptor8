
namespace trans {

const char *GetTranslation(const char *s);
void Destroy();

} // namespace trans

// _TR() marks strings that need to be translated
#define _TR(x) trans::GetTranslation(x)

// _TRN() marks strings that need to be translated but are used in a context
// that doesn't allow calling Trans::GetTranslation() (e.g. when used as part
// of a struct). This allows the translation manager script to see the string
// but they'll need additional code that does Trans::GetTranslation() on them
#define _TRN(x) (x)
