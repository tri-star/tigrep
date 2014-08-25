#ifndef TIGREP_GREP_CONFIG_BUILDER_H_
#define TIGREP_GREP_CONFIG_BUILDER_H_

#include "grep_command.h"
#include "application_config.h"
#include "log_type_repository.h"

namespace tigrep {

  class GrepConfigBuilder {

  public:

    GrepConfigBuilder() {

    }

    virtual ~GrepConfigBuilder() {

    }

    void build(const ApplicationConfig& app_config, const LogTypeRepository& log_type_repository, GrepConfig* grep_config) {

    }

  };

} //namespace

#endif
