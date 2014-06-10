#ifndef HEADSET_H
#define HEADSET_H

#include "edk.h"
#include <vector>

/* headset holds a list of channels, and a data buffer to
 * store information recieved from those channels.
 */

class headset
{
  std::vector<EE_DataChannels_enum> channels;
  int num_channels;
  double** data_buffer;
  unsigned int num_samples;
  int current_index;

  public:

    /* The default constructor yeilds a headset that is
     * listening to ED_TIMESTAMP, and an empty data buffer.
     */
    headset();
    ~headset();

    /* channel_exists(C) returns true if chanel C is being 
     * listened to by the headset.
     */
    bool channel_exists(EE_DataChannels_enum C);

    /* channel_add(C) attempts to add channel C to the list 
     * of channels. If C is succesfully added, 0 is returned. 
     * If C is already being listened to or the list of 
     * channels is full -1 is returned.
     */
    int channel_add(EE_DataChannels_enum C);

    /* channel_remove(C) attempts to remove channel C from
     * the list of channels. If C is succesfully removed, 0 is
     * returned. If C is not a valid channel for removal -1
     * is returned.
     */
    int channel_remove(EE_DataChannels_enum C);

    /* channel_get(n) returns the channel at location n in
     * the channel list.
     */
    EE_DataChannels_enum channel_get(int n);

    /* channel_write(f) writes the list of channels currently
     * being listened to on one line separated by a comma and
     * a space.
     */
    void channel_CSV_write(FILE* f);

    /* data_capture(num_sig, hData) clears the data buffer of
     * its current contents and replaces it with the info in
     * hData. num_sig defines the number of signals for each
     * channel being stored into the data buffer.
     */
    void data_capture(unsigned int num_sig, DataHandle& hData);

    /* data_CSV_write(f) writes the info stored in the data
     * buffer into f. For every signal number, the contents of
     * every channel currently being listened to are written on
     * one line separated by a comma and a space.
     */
    void data_CSV_write(FILE* f);

    int get_num_samples();

    int get_num_channels();

    int get_buffer_data(int y, int i);

    int get_current_index();

    void set_current_index(int i);

    void manual_data_entry();

    void set_num_channels(int i);

};

#endif
