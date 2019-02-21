## Handling key states -- Beyond the poll

Here are 3 methods for handling key-states which will help obtain more information from the keys:
- **synchronous keystates**: Polling the keystate and storing it to a variable for the duration of the needed state (until the next poll)
- **transitional keystates**: Tracks the current key state and the key state from the previous poll, effectively 1 frame in the past. This allows for checking of additional keystates than simply pressed or not.
- **tribools**: Normalises the states of two keys to a 3-state output, where the assigned variable can either be -1, 0, or 1. This makes it useful for the directional pas for navigation and other situations with up/down-type control.

### Transitional keystates:
Using the below snippets, one can check for a state transition, key press hold, key now pressed, key now released.

```c
// Key is changing state.
inline u32 key_transit(u32 key)
{   return ( __key_curr ^  __key_prev) & key;   }

// Key is held (down now and before).
inline u32 key_held(u32 key)
{   return ( __key_curr &  __key_prev) & key;  }

// Key is being hit (down now, but not before).
inline u32 key_hit(u32 key)
{   return ( __key_curr &~ __key_prev) & key;  }

//Key is being released (up now but down before)
inline u32 key_released(u32 key)
{   return (~__key_curr &  __key_prev) & key;  }
```