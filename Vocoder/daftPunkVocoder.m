function output = channel_vocoder(carrier, modul, channels, numberOfBands, overlap)
% y = channel_vocoder(carrier, modul, chan, numband, overlap)
% The Channel Vocoder modulates the carrier signal with the modulation
% signal in discrete channel amounts
% chan = number of channels         @ 256 --> 2048
% numband = number of bands (<chan) @ 8 --> 128
% overlap = window overlap          @ 1/64 --> 1/2

if numberOfBands>channels, error('# bands must be < # channels'), end
[rowOfCarrier, columnsOfCarrier]   = size(carrier);
if columnsOfCarrier>rc, carrier = carrier'; end
[rowOfModulator, columnsOfModulator]   = size(modul);   
if cm>rowOfModulator, modul = modul'; end

% allocate memory for future variables
len        = min(length(carrier),length(modul)); % find shortest length of both
carrier    = carrier(1:len,1:st);                % shorten carrier if needed
modul      = modul(1:len,1:st);                  % shorten modulator if needed
L          = 2*channels;                             % window length/FFT length
window     = hanning(L);                         % windowing function length L
bands      = 1:round(channels/numberOfBands):channels;         % indices for frequency bands     
bands(end) = channels;
output     = zeros(len,st);                      % output vector
iterator   = 0;


while iterator*L*overlap+L <= len
    index    = round([1+iterator*L*overlap:iterator*L*overlap+L]);
    FFTmod = fft( modul(index,:) .* window );    % window & take FFT of modulator
    FFTcar = fft( carrier(index,:) .* window );  % window & take FFT of carrier  
    total    = zeros(channels,st);              % zero pad the input
    
    % if total~= length(FFTmod*.FFTcar == size(total)) end;
    
    % credits to William Sethares for the implementation of an optimized
    % fast convolution in MATLAB that we was referenced for .wav files
    
    for jj = 1:numberOfBands-1                  % for each frequency band
        b        = [bands(jj):bands(jj+1)-1]; % current band
        total(b,:) = FFTcar(b,:)*diag(mean(abs(FFTmod(b,:)))); % orthogonal mult
    end                                   % take product of spectra
    midval   = FFTmod(1+L/2,:).*FFTcar(1+L/2,:); % start adding at midpoint
    mag  = [total; midval; flipud( conj( total(2:end,:) ) );]; % + and - frequencies
    timsig   = real( ifft(mag) );     % invert back to time
    output(index,:) = output(index,:) + timsig;         % add back into time waveform   
    iterator       = iterator+1;
end
output = 0.8*output/max(max(abs(output)));               % normalize output to prevent stereo clipping out of the soundcard