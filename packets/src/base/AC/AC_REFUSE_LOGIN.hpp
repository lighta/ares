struct AC_REFUSE_LOGIN {
  void emplace(const uint8_t ErrorCode, const char* blockDate, const size_t blockDate_sz) {
    PacketType = packet_id::AC_REFUSE_LOGIN;
    ErrorCode_ = ErrorCode;
    copy_buf_with_zero_pad(blockDate_, sizeof(blockDate_), blockDate, blockDate_sz);
  }

  void emplace(const uint8_t ErrorCode, const std::string& blockDate) {
    emplace(ErrorCode, blockDate.c_str(), blockDate.size() + 1);
  }

  void emplace(const uint8_t ErrorCode) {
    ErrorCode_ = ErrorCode;
    memset(blockDate_, 0, sizeof(blockDate_));
  }
  
  explicit AC_REFUSE_LOGIN(const uint8_t ErrorCode, const char* blockDate, const size_t blockDate_sz) {
    emplace(ErrorCode, blockDate, blockDate_sz);
  }

  explicit AC_REFUSE_LOGIN(const uint8_t ErrorCode, const std::string& blockDate) {
    emplace(ErrorCode, blockDate);
  }
  
  explicit AC_REFUSE_LOGIN(const uint8_t ErrorCode) {
    emplace(ErrorCode);
  }
                      
  uint16_t PacketType;
private:
  uint8_t ErrorCode_;
  char blockDate_[20];
};